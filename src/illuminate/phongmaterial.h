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

	// getters
	Vec3f getSpecularColor() const { return specularColor; }
	Vec3f getReflectiveColor() const { return reflectiveColor; }
	Vec3f getTransparentColor() const { return transparentColor; }
	float getIndexOfRefraction() const { return indexOfRefraction; }


	virtual Vec3f Shade(const Ray &ray, const Hit &hit,
						const Vec3f &dirToLight, const Vec3f &lightColor) const;
	virtual void glSetMaterial() const;
	virtual PhongMaterial* getPhongMaterial(Vec3f) { return this; }
};

#endif // PHONGMATERIAL_H
