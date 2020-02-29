#include "raytracer.h"

#include "light.h"
#include "rayTree.h"
#include "raytracing_stats.h"
#include "phongmaterial.h"
#include "marchinginfo.h"

#include "group.h"
#include "camera.h"

#include "scene_parser.h"

#include <set>

RayTracer::RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadows, bool shade_back,
					 Grid *grid, bool visualize_grid):
	sceneParser(s), max_bounces(max_bounces), cutoff_weight(cutoff_weight),
	shadows(shadows), shade_back(shade_back), visualize_grid(visualize_grid),
	grid(grid)
{
	if(grid) {
		sceneParser->getGroup()->insertIntoGrid(grid);
	}
}

RayTracer::~RayTracer()
{

}

Vec3f RayTracer::traceRay(const Ray &ray, float tmin, int bounces, float weight,
						  float indexOfRefraction, Hit &hit) const
{
	Vec3f ret;
	if(!traceRay_test(ret, ray, tmin, bounces, weight, 
		indexOfRefraction, hit, RayTree::SetMainSegment, false))
		ret = sceneParser->getBackgroundColor();
	return ret;
}

bool RayTracer::traceRay_test(Vec3f &color, const Ray &ray, float tmin, int bounces, 
							  float weight, float indexOfRefraction, Hit &hit,
							  void (*RayTree_Func)(const Ray &, float, float), bool isShadowRay) const
{
	bool ret;
	if (grid && !visualize_grid) {
		ret = RayCastFast(color, ray, tmin, bounces, weight, 
			indexOfRefraction, hit, RayTree_Func, isShadowRay);
	} else {
		ret = RayCast(color, ray, tmin, bounces, weight, 
			indexOfRefraction, hit, RayTree_Func, isShadowRay);
	}
	return ret;
}

bool RayTracer::RayCastFast(Vec3f &color, const Ray &ray, float tmin, int bounces, 
							float weight, float indexOfRefraction, Hit &hit,
							void (*RayTree_Func)(const Ray &, float, float), bool isShadowRay) const
{
	RayTracingStats::IncrementNumNonShadowRays();

	MarchingInfo info;
	grid->initializeRayMarch(info, ray, tmin);

	std::set<const Object3D*> objSet;
	float minHitT = FLT_MAX;
	Hit dummy;
	for (; info.hasHit && grid->inGrid(info.index); info.nextCell()) {
		RayTracingStats::IncrementNumGridCellsTraversed();

		Voxel curVoxel = grid->getVoxelAt(info.index);
		for (const Object3D* obj : curVoxel) {
			if (objSet.find(obj) == objSet.end()) {
				objSet.insert(obj);
				if (obj->intersect(ray, dummy, tmin) && dummy.getT() < minHitT) {
					minHitT = dummy.getT();
					hit = dummy;
				}
			}
		}
		if (minHitT < info.nextT_f())
			break;
	}
	// test planes
	for(const Object3D* obj : grid->getExtras()) {
		if(obj->intersect(ray, dummy, tmin) && dummy.getT() < minHitT) {
			minHitT = dummy.getT();
			hit = dummy;
		}
	}

	if (minHitT == FLT_MAX) {
		RayTree_Func(ray, tmin, INF);
		return false;
	} else {
		RayTree_Func(ray, tmin, hit.getT());
		if (!isShadowRay) {
			color = weight * getHitColor(ray, hit, bounces, weight, indexOfRefraction);
		}
		return true;
	}
}

bool RayTracer::RayCast(Vec3f &color, const Ray &ray, float tmin, int bounces, 
						float weight, float indexOfRefraction, Hit &hit,
						void (*RayTree_Func)(const Ray &, float, float), bool isShadowRay) const
{
	RayTracingStats::IncrementNumNonShadowRays();

	const Object3D* group = sceneParser->getGroup();
	if(visualize_grid)
		group = grid;

	if(!group->intersect(ray, hit, tmin)) {
		RayTree_Func(ray, tmin, INF);
		return false;
	} else {
		RayTree_Func(ray, tmin, hit.getT());
		if (!isShadowRay) {
			color = weight * getHitColor(ray, hit, bounces, weight, indexOfRefraction);
		}
		return true;
	}
}

Vec3f RayTracer::getHitColor(const Ray &ray, Hit &hit, int bounces, float weight, float indexOfRefraction) const
{
	Vec3f ret;

	Vec3f hitPoint = hit.getIntersectionPoint();
	Material* material = hit.getMaterial();
	if(shade_back && hit.getNormal().Dot3(ray.getDirection()) > 0) {
		hit.set(hit.getT(), hit.getMaterial(), -1 * hit.getNormal(), ray);
	}

	// add ambient illumination
	ret = sceneParser->getAmbientLight() * material->getDiffuseColor(hitPoint);
	// add light illumination
	for(int i=0; i<sceneParser->getNumLights(); i++) {
		Light* light = sceneParser->getLight(i);

		// get light info
		Vec3f dirToLight, lightColor;
		float dist;
		light->getIllumination(hitPoint, dirToLight, lightColor, dist);

		// add local illumination with shadow
		Ray rayToLight(hitPoint, dirToLight);
		bool blocked = traceShadowRay(rayToLight, dist);
		if(!blocked) {
			ret += material->Shade(ray, hit, dirToLight, lightColor);
		}
	}

	// trace ray by recursive
	if(bounces < max_bounces && weight > cutoff_weight) {
		Vec3f normal = hit.getNormal();
		Vec3f incoming = ray.getDirection();
		float index = material->getIndexOfRefraction(hitPoint);
		float materialIndex = std::abs(index - indexOfRefraction) < epsilon?
								Material::indexOfVacuum : index;

		Hit dummy;
		// add reflection ray
		Vec3f reflectiveColor = material->getReflectiveColor(hitPoint);
		if(reflectiveColor.Length() > 0) {
			Vec3f reflection = Vec3f::mirrorDirection(normal, incoming);
			float nowWeight = weight;

			Ray reflection_ray(hitPoint, reflection);
			Vec3f color;
			if(!traceRay_test(color, reflection_ray, epsilon, bounces + 1, nowWeight,
							  materialIndex, dummy, RayTree::AddReflectedSegment, false)) {
				color = sceneParser->getBackgroundColor();
			}
			ret += reflectiveColor * color;
		}

		// add refraction ray
		Vec3f transparentColor = material->getTransparentColor(hitPoint);
		if(transparentColor.Length() > 0) {
			Vec3f refraction;
			bool pass = Vec3f::transmittedDirection(
				normal, incoming, indexOfRefraction,
				materialIndex, refraction
			);
			float nowWeight = weight;

			Ray refraction_ray(hitPoint, refraction);
			if(pass) {
				Vec3f color;
				if (!traceRay_test(color, refraction_ray, epsilon, bounces + 1, nowWeight,
								   materialIndex, dummy, RayTree::AddTransmittedSegment, false)) {
					color = sceneParser->getBackgroundColor();
				}
				ret += transparentColor * color;
			}
		}

	}
	return ret;
}

bool RayTracer::traceShadowRay(Ray &ray, float dist) const
{
	bool ret = false;
	Hit hit;
	if(shadows) {
		RayTracingStats::IncrementNumShadowRays();
		Vec3f color;
		bool blocked = traceRay_test(color, ray, epsilon, max_bounces, 
				cutoff_weight, 0, hit, RayTree::AddShadowSegment, true);

		ret = blocked && (hit.getT() < dist);
	}
	return ret;
}
