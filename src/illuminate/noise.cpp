#include "noise.h"
#include "phongmaterial.h"

Noise::Noise(Matrix *m, Material *mat1, Material *mat2, int octaves):
	Material((mat1->getDiffuseColor() + mat2->getDiffuseColor()) / 2),
	material1(mat1), material2(mat2), octaves(octaves)
{
	matrix = *m;
}

Vec3f Noise::Shade(const Ray &ray, const Hit &hit,
						  const Vec3f &dirToLight, const Vec3f &lightColor) const
{
	Vec3f hitPoint = hit.getIntersectionPoint();
	hitPoint = matrix(hitPoint);

	return material1->Shade(ray, hit, dirToLight, lightColor);
}

void Noise::glSetMaterial() const
{
	material1->glSetMaterial();
}

PhongMaterial *Noise::getPhongMaterial(Vec3f hitPoint)
{
	return (PhongMaterial*)material1;
}
