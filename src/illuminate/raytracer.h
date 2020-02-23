#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "ray.h"
#include "hit.h"

#include "grid.h"
//#include "scene_parser.h"

#include <set>

typedef Vec3f Color;
class SceneParser;

class RayTracer
{
public:
	constexpr static float epsilon = 1e-4f;
	constexpr static float INF = 1e6f;
	constexpr static float indexOfVacuum = 1.0f;

private:
	SceneParser *sceneParser;
	int max_bounces;
	float cutoff_weight;
	bool shadows;
	bool shade_back;
	bool visualize_grid;

	Grid* grid;
public:
	RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadows, bool shade_back,
			  Grid* grid, bool visualize_grid);
	~RayTracer();

	Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight,
					 float indexOfRefraction, Hit &hit) const;
private:
	Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight,
							  float indexOfRefraction, Hit &hit,
							  void (*RayTree_Func)(const Ray &, float, float)) const;
	Vec3f RayCast(Ray &ray, float tmin, int bounces, float weight,
					float indexOfRefraction, Hit &hit,
					void (*RayTree_Func)(const Ray&, float, float)) const;
	Vec3f RayCastFast(Ray &ray, float tmin, int bounces, float weight,
							  float indexOfRefraction, Hit &hit,
							  void (*RayTree_Func)(const Ray &, float, float)) const;
	bool intersectVoxel(const Voxel &voxel, Ray &ray, float tmin, Hit &hit) const;
	Vec3f getHitColor(Ray &ray, Hit &hit, int bounces, float weight, float indexOfRefraction) const;
	bool traceShadowRay(Ray &ray, float dist) const;
};

#endif // RAYTRACER_H
