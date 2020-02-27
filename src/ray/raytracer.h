#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "ray.h"
#include "hit.h"

#include "grid.h"

#include <set>

class SceneParser;

class RayTracer
{
public:
	constexpr static float epsilon = 1e-4f;
	constexpr static float INF = 1e6f;

private:
	SceneParser *sceneParser;
	int max_bounces;
	float cutoff_weight;
	bool shadows;
	bool shade_back;
	bool visualize_grid;

	const Grid* grid;
public:
	RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadows, bool shade_back,
			  Grid* grid, bool visualize_grid);
	~RayTracer();

	Vec3f traceRay(const Ray &ray, float tmin, int bounces, float weight,
					 float indexOfRefraction, Hit &hit) const;
private:
	bool traceRay_test(Vec3f &color, const Ray &ray, float tmin, int bounces, float weight,
							  float indexOfRefraction, Hit &hit,
							  void (*RayTree_Func)(const Ray &, float, float)) const;
	bool RayCast(Vec3f &color, const Ray &ray, float tmin, int bounces, float weight,
					float indexOfRefraction, Hit &hit,
					void (*RayTree_Func)(const Ray&, float, float)) const;
	bool RayCastFast(Vec3f &color, const Ray &ray, float tmin, int bounces, float weight,
							  float indexOfRefraction, Hit &hit,
							  void (*RayTree_Func)(const Ray &, float, float)) const;
	bool intersectVoxel(const Voxel &voxel, Ray &ray, float tmin, Hit &hit) const;
	Vec3f getHitColor(const Ray &ray, Hit &hit, int bounces, float weight, float indexOfRefraction) const;
	bool traceShadowRay(Ray &ray, float dist) const;
};

#endif // RAYTRACER_H
