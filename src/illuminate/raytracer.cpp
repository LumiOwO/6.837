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

static std::set<const Object3D*> objSet;
static bool isTracingShadow;

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight,
						  float indexOfRefraction, Hit &hit) const
{
	isTracingShadow = false;

	Vec3f ret = traceRay(ray, tmin, bounces, weight, indexOfRefraction, hit, RayTree::SetMainSegment);
	if(ret[0] < 0)
		ret = sceneParser->getBackgroundColor();
	return ret;
}

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight,
						  float indexOfRefraction, Hit &hit,
						  void (*RayTree_Func)(const Ray &, float, float)) const
{
	objSet.clear();
	Vec3f ret;
	if(grid && !visualize_grid)
		ret = RayCastFast(ray, tmin, bounces, weight, indexOfRefraction, hit, RayTree_Func);
	else
		ret = RayCast(ray, tmin, bounces, weight, indexOfRefraction, hit, RayTree_Func);
	return ret;
}

Vec3f RayTracer::RayCastFast(Ray &ray, float tmin, int bounces, float weight,
						  float indexOfRefraction, Hit &hit,
						  void (*RayTree_Func)(const Ray &, float, float)) const
{
	RayTracingStats::IncrementNumNonShadowRays();

	Color buffer(-1, -1, -1);
	float minHitT = FLT_MAX;
	Hit dummy;

	MarchingInfo info;
	grid->initializeRayMarch(info, ray, tmin);
	int i = info.getIndexAt(0);
	int j = info.getIndexAt(1);
	int k = info.getIndexAt(2);

	while(info.hasHit()
		  && i >= 0 && i <= grid->getNx()
		  && j >= 0 && j <= grid->getNy()
		  && k >= 0 && k <= grid->getNz())
	{
//		cout << "(" << i << ", " << j << ", " << k << ")" << endl;
		if(i == grid->getNx() || j == grid->getNy() || k == grid->getNz()) {
			;
		} else {
			Voxel curVoxel = grid->getVoxelAt(i, j, k);
			if(curVoxel.size() > 0) {
				for(const Object3D* obj : curVoxel)
				{
					if(objSet.find(obj) == objSet.end()) {
						objSet.insert(obj);
						if(obj->intersect(ray, dummy, tmin) && dummy.getT() < minHitT) {
							minHitT = dummy.getT();
							hit = dummy;
						}
					}
				}
			}

			if(minHitT < FLT_MAX)
				break;
		}
		// to next cell
		info.nextCell();
		RayTracingStats::IncrementNumGridCellsTraversed();
		i = info.getIndexAt(0);
		j = info.getIndexAt(1);
		k = info.getIndexAt(2);
	}

	for(const Object3D* obj : grid->getExtras())
	{
		if(obj->intersect(ray, dummy, tmin) && dummy.getT() < minHitT) {
			minHitT = dummy.getT();
			hit = dummy;
		}
	}

	if(minHitT >= FLT_MAX)
	{
		RayTree_Func(ray, tmin, INF);
	}
	else
	{
		RayTree_Func(ray, tmin, hit.getT());
		buffer = getHitColor(ray, hit, bounces, weight, indexOfRefraction);
	}

	return weight * buffer;
}

Vec3f RayTracer::RayCast(Ray &ray, float tmin, int bounces, float weight,
						  float indexOfRefraction, Hit &hit,
						  void (*RayTree_Func)(const Ray &, float, float)) const
{
	RayTracingStats::IncrementNumNonShadowRays();

	Color buffer(-1, -1, -1);

	Object3D* group = sceneParser->getGroup();
	if(visualize_grid)
		group = grid;

	if(!group->intersect(ray, hit, tmin))
	{
		RayTree_Func(ray, tmin, INF);
	}
	else
	{
		RayTree_Func(ray, tmin, hit.getT());
		buffer = getHitColor(ray, hit, bounces, weight, indexOfRefraction);
	}
	return weight * buffer;
}

Vec3f RayTracer::getHitColor(Ray &ray, Hit &hit, int bounces, float weight, float indexOfRefraction) const
{
	Vec3f ret;

	Vec3f hitPoint = hit.getIntersectionPoint();
	Material* material = hit.getMaterial();
	if(shade_back && hit.getNormal().Dot3(ray.getDirection()) > 0) {
		hit.set(hit.getT(), hit.getMaterial(), -1 * hit.getNormal(), ray);
	}

	// add ambient illumination
	ret = sceneParser->getAmbientLight() * material->getDiffuseColor();
	// add light illumination
	for(int i=0; i<sceneParser->getNumLights(); i++)
	{
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
		PhongMaterial* phongMaterial = material->getPhongMaterial(hitPoint);
		Vec3f normal = hit.getNormal();
		Vec3f incoming = ray.getDirection();
		float materialIndex = fabsf(phongMaterial->getIndexOfRefraction() - indexOfRefraction) < epsilon?
								  indexOfVacuum: phongMaterial->getIndexOfRefraction();

		Hit dummy;
		// add reflection ray
		if(phongMaterial->getReflectiveColor().Length() > 0) {
			Vec3f reflection = Vec3f::mirrorDirection(normal, incoming);
			float nowWeight = weight;

			Ray reflection_ray(hitPoint, reflection);
			Vec3f color = traceRay(reflection_ray, epsilon, bounces + 1,
								   nowWeight, materialIndex, dummy, RayTree::AddReflectedSegment);
			if(color[0] < 0)
				color = sceneParser->getBackgroundColor();
			ret += phongMaterial->getReflectiveColor() * color;
		}

		// add refraction ray
		if(phongMaterial->getTransparentColor().Length() > 0) {
			Vec3f refraction;
			bool pass = Vec3f::transmittedDirection(
				normal, incoming, indexOfRefraction,
				materialIndex, refraction
			);
			float nowWeight = weight;

			Ray refraction_ray(hitPoint, refraction);
			if(pass) {
				Vec3f color = traceRay(refraction_ray, epsilon, bounces + 1,
									   nowWeight, materialIndex, dummy, RayTree::AddTransmittedSegment);
				if(color[0] < 0)
					color = sceneParser->getBackgroundColor();
				ret += phongMaterial->getTransparentColor() * color;
			}
		}

	}
	return ret;
}

bool RayTracer::traceShadowRay(Ray &ray, float dist) const
{
	bool ret = false;
	Hit hit;
	if(shadows && !isTracingShadow) {
		RayTracingStats::IncrementNumShadowRays();

		isTracingShadow = true;
		Vec3f color = traceRay(ray, epsilon, max_bounces, cutoff_weight, 0, hit, RayTree::AddShadowSegment);
		isTracingShadow = false;

		ret = (color[0] >= 0) && (hit.getT() < dist);
	}
	return ret;
}
