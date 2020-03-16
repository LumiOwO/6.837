#ifndef _VERTICALFORCEFIELD_H_
#define _VERTICALFORCEFIELD_H_

#include "ForceField.h"

class VerticalForceField: public ForceField
{
private:
	float magnitude;
public:
	VerticalForceField(float magnitude): magnitude(magnitude) {}
	virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override {
		return Vec3f(0, -position.y(), 0) * magnitude / mass;
	}
};

#endif
