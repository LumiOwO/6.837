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
	virtual ~Noise() {}

	virtual Vec3f getDiffuseColor(Vec3f p) const override;
	virtual Vec3f getReflectiveColor(Vec3f p) const override;
	virtual Vec3f getTransparentColor(Vec3f p) const override;
	virtual float getIndexOfRefraction(Vec3f p) const override;

	virtual Vec3f Shade(const Ray &ray, const Hit &hit,
						const Vec3f &dirToLight, const Vec3f &lightColor) const override;
	virtual void glSetMaterial() const override;

	static float N(float x, float y, float z, int octaves);
};

#endif // NOISE_H
