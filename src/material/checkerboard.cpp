#include "checkerboard.h"

CheckerBoard::CheckerBoard(Matrix *m, Material *mat1, Material *mat2):
	Material((mat1->getDiffuseColor() + mat2->getDiffuseColor()) / 2),
	material1(mat1), material2(mat2), matrix(*m)
{

}

Vec3f CheckerBoard::getDiffuseColor(Vec3f p) const
{
	return odd(matrix(p))?
		material1->getDiffuseColor(p):
		material2->getDiffuseColor(p);
}

Vec3f CheckerBoard::getReflectiveColor(Vec3f p) const
{
	return odd(matrix(p)) ?
		material1->getReflectiveColor(p) :
		material2->getReflectiveColor(p);
}

Vec3f CheckerBoard::getTransparentColor(Vec3f p) const
{
	return odd(matrix(p)) ?
		material1->getTransparentColor(p) :
		material2->getTransparentColor(p);
}

float CheckerBoard::getIndexOfRefraction(Vec3f p) const
{
	return odd(matrix(p)) ?
		material1->getIndexOfRefraction(p) :
		material2->getIndexOfRefraction(p);
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

bool CheckerBoard::odd(const Vec3f &pos) const
{
	bool ret = false;
	for(int i=0; i<3; i++) {
		ret ^= ((int)floor(pos[i]) & 0x1);
	}
	return ret;
}
