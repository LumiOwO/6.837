#include "vectors.h"
#include "raytracer.h"

Vec3f Vec3f::mirrorDirection(const Vec3f &normal, const Vec3f &incoming)
{
	Vec3f vertical = normal.Dot3(incoming) * normal;
	return incoming - 2 * vertical;
}

bool Vec3f::transmittedDirection(const Vec3f &normal, const Vec3f &incoming,
								 float index_i, float index_t, Vec3f &transmitted)
{
	float cos_i = -normal.Dot3(incoming);
	bool isEnter = cos_i > 0;
	cos_i = fabsf(cos_i);

	float n_r = index_i / index_t;
	float cos_j2 = 1.0f - (1.0f - cos_i * cos_i) * (n_r * n_r);
	if(cos_j2 < 0)
		return false;

	transmitted = (isEnter? 1: -1) * (n_r * cos_i - sqrtf(cos_j2)) * normal
						+ n_r * incoming;
	transmitted = transmitted.Normalize();
	return true;
}
