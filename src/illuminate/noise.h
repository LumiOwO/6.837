#ifndef NOISE_H
#define NOISE_H

#include "material.h"
#include "matrix.h"

class Noise: public Material
{
private:
	Matrix matrix;
	Material *material1;
	Material *material2;
	int octaves;
public:
	Noise(Matrix *m, Material *mat1, Material *mat2, int octaves);

	virtual Vec3f Shade(const Ray &ray, const Hit &hit,
						const Vec3f &dirToLight, const Vec3f &lightColor) const;

	virtual void glSetMaterial() const;
	virtual PhongMaterial* getPhongMaterial(Vec3f hitPoint);
};

#endif // NOISE_H
