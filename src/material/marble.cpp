#include "marble.h"
#include "noise.h"

Marble::Marble(Matrix *m, Material *mat1, Material *mat2,
			   int octaves, float frequency, float amplitude):
	Material((mat1->getDiffuseColor() + mat2->getDiffuseColor()) / 2),
	material1(mat1), material2(mat2), octaves(octaves),
	frequency(frequency), amplitude(amplitude), matrix(*m)
{

}

Vec3f Marble::getDiffuseColor(Vec3f p) const
{
	Vec3f hitPoint = matrix(p);
	float m = M(hitPoint.x(), hitPoint.y(), hitPoint.z(), octaves, frequency, amplitude);
	return m * material1->getDiffuseColor(p)
		+ (1 - m) * material2->getDiffuseColor(p);
}

Vec3f Marble::getReflectiveColor(Vec3f p) const
{
	Vec3f hitPoint = matrix(p);
	float m = M(hitPoint.x(), hitPoint.y(), hitPoint.z(), octaves, frequency, amplitude);
	return m * material1->getReflectiveColor(p)
		+ (1 - m) * material2->getReflectiveColor(p);
}

Vec3f Marble::getTransparentColor(Vec3f p) const
{
	Vec3f hitPoint = matrix(p);
	float m = M(hitPoint.x(), hitPoint.y(), hitPoint.z(), octaves, frequency, amplitude);
	return m * material1->getTransparentColor(p)
		+ (1 - m) * material2->getTransparentColor(p);
}

float Marble::getIndexOfRefraction(Vec3f p) const
{
	Vec3f hitPoint = matrix(p);
	float m = M(hitPoint.x(), hitPoint.y(), hitPoint.z(), octaves, frequency, amplitude);
	return m * material1->getIndexOfRefraction(p)
		+ (1 - m) * material2->getIndexOfRefraction(p);
}

Vec3f Marble::Shade(const Ray &ray, const Hit &hit,
						  const Vec3f &dirToLight, const Vec3f &lightColor) const
{
	Vec3f hitPoint = hit.getIntersectionPoint();
	hitPoint = matrix(hitPoint);

	float m = M(hitPoint.x(), hitPoint.y(), hitPoint.z(), octaves, frequency, amplitude);
	return m * material1->Shade(ray, hit, dirToLight, lightColor)
		+ (1 - m) * material2->Shade(ray, hit, dirToLight, lightColor);
}

void Marble::glSetMaterial() const
{
	material1->glSetMaterial();
}

float Marble::M(float x, float y, float z, int octaves, float f, float amp)
{
	return std::sin(f * x + amp * Noise::N(x, y, z, octaves));
}
