#ifndef _GRAVITYFORCEFIELD_H_
#define _GRAVITYFORCEFIELD_H_

#include "ForceField.h"

class GravityForceField: public ForceField
{
private:
	Vec3f g;
public:
	GravityForceField(Vec3f gravity): g(gravity) {}
	virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override {
		return g;
	}
};

#endif
