#ifndef _CONSTANTFORCEFIELD_H_
#define _CONSTANTFORCEFIELD_H_

#include "ForceField.h"

class ConstantForceField: public ForceField 
{
private:
	Vec3f F;
public:
	ConstantForceField(Vec3f force): F(force) {}
	virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override {
		return F / mass;
	}
};

#endif
