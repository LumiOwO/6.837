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
	virtual ~CheckerBoard() {}

	virtual Vec3f getDiffuseColor(Vec3f p) const override;
	virtual Vec3f getReflectiveColor(Vec3f p) const override;
	virtual Vec3f getTransparentColor(Vec3f p) const override;
	virtual float getIndexOfRefraction(Vec3f p) const override;

	virtual Vec3f Shade(const Ray &ray, const Hit &hit,
						const Vec3f &dirToLight, const Vec3f &lightColor) const override;
	virtual void glSetMaterial() const override;
private:
	bool odd(const Vec3f& pos) const;
};

#endif // CHECKERBOARD_H
