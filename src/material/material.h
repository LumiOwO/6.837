#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include "hit.h"
#include "ray.h"

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:
	constexpr static float indexOfVacuum = 1.0f;

	// CONSTRUCTORS & DESTRUCTOR
	Material(const Vec3f &d_color) { diffuseColor = d_color; }
	virtual ~Material() {}

	// ACCESSORS
	virtual Vec3f getDiffuseColor(Vec3f p = Vec3f()) const { return diffuseColor; }
	virtual Vec3f getReflectiveColor(Vec3f p = Vec3f()) const { return Vec3f(0, 0, 0); }
	virtual Vec3f getTransparentColor(Vec3f p = Vec3f()) const { return Vec3f(0, 0, 0); }
	virtual float getIndexOfRefraction(Vec3f p = Vec3f()) const { return indexOfVacuum; }

	virtual Vec3f Shade(const Ray &ray, const Hit &hit,
						const Vec3f &dirToLight, const Vec3f &lightColor) const = 0;
	virtual void glSetMaterial() const = 0;

protected:
	// REPRESENTATION
	Vec3f diffuseColor;

};

// ====================================================================
// ====================================================================

#endif
