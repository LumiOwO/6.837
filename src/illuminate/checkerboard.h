#ifndef CHECKERBOARD_H
#define CHECKERBOARD_H

#include "material.h"
#include "matrix.h"

class CheckerBoard: public Material
{
private:
	Matrix matrix;
	Material *material1;
	Material *material2;
public:
	CheckerBoard(Matrix *m, Material *mat1, Material *mat2);

	virtual Vec3f Shade(const Ray &ray, const Hit &hit,
						const Vec3f &dirToLight, const Vec3f &lightColor) const;

	virtual void glSetMaterial() const;
	virtual PhongMaterial* getPhongMaterial(Vec3f hitPoint);
private:
	bool odd(const Vec3f& pos) const;
};

#endif // CHECKERBOARD_H
