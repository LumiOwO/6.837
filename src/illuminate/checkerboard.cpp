#include "checkerboard.h"
#include "phongmaterial.h"

CheckerBoard::CheckerBoard(Matrix *m, Material *mat1, Material *mat2):
	Material((mat1->getDiffuseColor() + mat2->getDiffuseColor()) / 2),
	material1(mat1), material2(mat2)
{
	matrix = *m;
}

Vec3f CheckerBoard::Shade(const Ray &ray, const Hit &hit,
						  const Vec3f &dirToLight, const Vec3f &lightColor) const
{
	Vec3f hitPoint = hit.getIntersectionPoint();
	hitPoint = matrix(hitPoint);

	return odd(hitPoint)?
			material1->Shade(ray, hit, dirToLight, lightColor):
			material2->Shade(ray, hit, dirToLight, lightColor);
}

void CheckerBoard::glSetMaterial() const
{
	material1->glSetMaterial();
}

PhongMaterial *CheckerBoard::getPhongMaterial(Vec3f hitPoint)
{
	hitPoint = matrix(hitPoint);
	return odd(hitPoint)? (PhongMaterial*)material1: (PhongMaterial*)material2;
}

bool CheckerBoard::odd(const Vec3f &pos) const
{
	bool ret = false;
	for(int i=0; i<3; i++) {
		ret ^= ((int)floor(pos[i]) & 0x1);
	}
	return ret;
}
