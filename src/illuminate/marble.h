#ifndef MARBLE_H
#define MARBLE_H

#include "material.h"
#include "matrix.h"

class Marble: public Material
{
private:
	Matrix matrix;
	Material *material1;
	Material *material2;
	int octaves;
	float frequency;
	float amplitude;
public:
	Marble(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude);

	virtual Vec3f Shade(const Ray &ray, const Hit &hit,
						const Vec3f &dirToLight, const Vec3f &lightColor) const;

	virtual void glSetMaterial() const;
	virtual PhongMaterial* getPhongMaterial(Vec3f hitPoint);
};

#endif // MARBLE_H
