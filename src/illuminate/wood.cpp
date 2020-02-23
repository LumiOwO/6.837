#include "wood.h"
#include "phongmaterial.h"

Wood::Wood(Matrix *m, Material *mat1, Material *mat2,
			   int octaves, float frequency, float amplitude):
	Material((mat1->getDiffuseColor() + mat2->getDiffuseColor()) / 2),
	material1(mat1), material2(mat2), octaves(octaves),
	frequency(frequency), amplitude(amplitude)
{
	matrix = *m;
}

Vec3f Wood::Shade(const Ray &ray, const Hit &hit,
						  const Vec3f &dirToLight, const Vec3f &lightColor) const
{
	Vec3f hitPoint = hit.getIntersectionPoint();
	hitPoint = matrix(hitPoint);

	return material1->Shade(ray, hit, dirToLight, lightColor);
}

void Wood::glSetMaterial() const
{
	material1->glSetMaterial();
}

PhongMaterial *Wood::getPhongMaterial(Vec3f hitPoint)
{
	return (PhongMaterial*)material1;
}
