#ifndef WOOD_H
#define WOOD_H

#include "material.h"
#include "matrix.h"

class Wood: public Material
{
private:
	Matrix matrix;
	Material *material1;
	Material *material2;
	int octaves;
	float frequency;
	float amplitude;
public:
	Wood(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude);

	virtual Vec3f Shade(const Ray &ray, const Hit &hit,
						const Vec3f &dirToLight, const Vec3f &lightColor) const;

	virtual void glSetMaterial() const;
	virtual PhongMaterial* getPhongMaterial(Vec3f hitPoint);
};

#endif // WOOD_H
