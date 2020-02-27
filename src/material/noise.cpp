#include "noise.h"
#include "perlin_noise.h"

Noise::Noise(Matrix *m, Material *mat1, Material *mat2, int octaves):
	Material((mat1->getDiffuseColor() + mat2->getDiffuseColor()) / 2),
	material1(mat1), material2(mat2), octaves(octaves), matrix(*m)
{

}

Vec3f Noise::getDiffuseColor(Vec3f p) const
{
	Vec3f hitPoint = matrix(p);
	float n = N(hitPoint.x(), hitPoint.y(), hitPoint.z(), octaves);
	return n * material1->getDiffuseColor(p)
		+ (1 - n) * material2->getDiffuseColor(p);
}

Vec3f Noise::getReflectiveColor(Vec3f p) const
{
	Vec3f hitPoint = matrix(p);
	float n = N(hitPoint.x(), hitPoint.y(), hitPoint.z(), octaves);
	return n * material1->getReflectiveColor(p)
		+ (1 - n) * material2->getReflectiveColor(p);
}

Vec3f Noise::getTransparentColor(Vec3f p) const
{
	Vec3f hitPoint = matrix(p);
	float n = N(hitPoint.x(), hitPoint.y(), hitPoint.z(), octaves);
	return n * material1->getTransparentColor(p)
		+ (1 - n) * material2->getTransparentColor(p);
}

float Noise::getIndexOfRefraction(Vec3f p) const
{
	Vec3f hitPoint = matrix(p);
	float n = N(hitPoint.x(), hitPoint.y(), hitPoint.z(), octaves);
	return n * material1->getIndexOfRefraction(p)
		+ (1 - n) * material2->getIndexOfRefraction(p);
}

Vec3f Noise::Shade(const Ray &ray, const Hit &hit,
						  const Vec3f &dirToLight, const Vec3f &lightColor) const
{
	Vec3f hitPoint = hit.getIntersectionPoint();
	hitPoint = matrix(hitPoint);

	float n = N(hitPoint.x(), hitPoint.y(), hitPoint.z(), octaves);
	return n * material1->Shade(ray, hit, dirToLight, lightColor)
		+ (1 - n) * material2->Shade(ray, hit, dirToLight, lightColor);
}

void Noise::glSetMaterial() const
{
	material1->glSetMaterial();
}

float Noise::N(float x, float y, float z, int octaves)
{
	float sum = 0;
	float mul = 1;
	for (int i = 0; i < octaves; i++) {
		sum += noise_f(x * mul, y * mul, z * mul) / mul;
		mul *= 2;
	}
	return sum;
}
