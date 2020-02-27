#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include "material.h"

class PhongMaterial: public Material
{
private:
	Vec3f specularColor;
	float exponent;

	Vec3f reflectiveColor;
	Vec3f transparentColor;
	float indexOfRefraction;

public:
	PhongMaterial(
			const Vec3f &diffuseColor,
			const Vec3f &specularColor,
			float exponent,
			const Vec3f &reflectiveColor,
			const Vec3f &transparentColor,
			float indexOfRefraction);
	virtual ~PhongMaterial() {}

	virtual Vec3f getReflectiveColor(Vec3f p = Vec3f()) const override { return reflectiveColor; }
	virtual Vec3f getTransparentColor(Vec3f p = Vec3f()) const override { return transparentColor; }
	virtual float getIndexOfRefraction(Vec3f p = Vec3f()) const override { return indexOfRefraction; }

	virtual Vec3f Shade(const Ray &ray, const Hit &hit,
						const Vec3f &dirToLight, const Vec3f &lightColor) const override;
	virtual void glSetMaterial() const override;
private:
	Vec3f getSpecularColor() const { return specularColor; }
};

#endif // PHONGMATERIAL_H
