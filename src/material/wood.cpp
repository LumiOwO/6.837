#include "wood.h"
#include "noise.h"

Wood::Wood(Matrix *m, Material *mat1, Material *mat2,
			   int octaves, float frequency, float amplitude):
	Material((mat1->getDiffuseColor() + mat2->getDiffuseColor()) / 2),
	material1(mat1), material2(mat2), octaves(octaves),
	frequency(frequency), amplitude(amplitude), matrix(*m)
{

}

Vec3f Wood::getDiffuseColor(Vec3f p) const
{
	Vec3f hitPoint = matrix(hitPoint);
	float w = W(hitPoint.x(), hitPoint.y(), hitPoint.z(), octaves, frequency, amplitude);
	return w * material1->getDiffuseColor(p)
		+ (1 - w) * material2->getDiffuseColor(p);
}

Vec3f Wood::getReflectiveColor(Vec3f p) const
{
	Vec3f hitPoint = matrix(hitPoint);
	float w = W(hitPoint.x(), hitPoint.y(), hitPoint.z(), octaves, frequency, amplitude);
	return w * material1->getReflectiveColor(p)
		+ (1 - w) * material2->getReflectiveColor(p);
}

Vec3f Wood::getTransparentColor(Vec3f p) const
{
	Vec3f hitPoint = matrix(hitPoint);
	float w = W(hitPoint.x(), hitPoint.y(), hitPoint.z(), octaves, frequency, amplitude);
	return w * material1->getTransparentColor(p)
		+ (1 - w) * material2->getTransparentColor(p);
}

float Wood::getIndexOfRefraction(Vec3f p) const
{
	Vec3f hitPoint = matrix(hitPoint);
	float w = W(hitPoint.x(), hitPoint.y(), hitPoint.z(), octaves, frequency, amplitude);
	return w * material1->getIndexOfRefraction(p)
		+ (1 - w) * material2->getIndexOfRefraction(p);
}

Vec3f Wood::Shade(const Ray &ray, const Hit &hit,
						  const Vec3f &dirToLight, const Vec3f &lightColor) const
{
	Vec3f hitPoint = hit.getIntersectionPoint();
	hitPoint = matrix(hitPoint);

	float w = W(hitPoint.x(), hitPoint.y(), hitPoint.z(), octaves, frequency, amplitude);
	return w * material1->Shade(ray, hit, dirToLight, lightColor)
		+ (1 - w) * material2->Shade(ray, hit, dirToLight, lightColor);
}

void Wood::glSetMaterial() const
{
	material1->glSetMaterial();
}

float Wood::W(float x, float y, float z, int octaves, float f, float amp)
{
	return sin(f * sqrt(x * x + z * z + 0.15f * y) + amp * Noise::N(x, 0.15f * y, z, octaves));
}
