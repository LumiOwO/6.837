#ifndef _RADIALFORCEFIELD_H_
#define _RADIALFORCEFIELD_H_

#include "ForceField.h"

class RadialForceField: public ForceField
{
private:
	float magnitude;
public:
	RadialForceField(float magnitude): magnitude(magnitude) {}
	virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override {
		return -position * magnitude / mass;
	}
};

#endif
