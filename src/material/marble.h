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
	virtual ~Marble() {}

	virtual Vec3f getDiffuseColor(Vec3f p) const override;
	virtual Vec3f getReflectiveColor(Vec3f p) const override;
	virtual Vec3f getTransparentColor(Vec3f p) const override;
	virtual float getIndexOfRefraction(Vec3f p) const override;

	virtual Vec3f Shade(const Ray &ray, const Hit &hit,
						const Vec3f &dirToLight, const Vec3f &lightColor) const override;
	virtual void glSetMaterial() const override;

	static float M(float x, float y, float z, int octaves, float f, float amp);
};

#endif // MARBLE_H
