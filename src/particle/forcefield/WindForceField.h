#ifndef _WINDFORCEFIELD_H_
#define _WINDFORCEFIELD_H_

#include "ForceField.h"
#include "perlin_noise.h"

class WindForceField: public ForceField
{
private:
	float magnitude;
public:
	WindForceField(float magnitude): magnitude(magnitude) {}
	virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override {
		Vec3f delta(
			noise_f(position.x() + t, position.y(), position.z()),
			0,
			noise_f(position.x(), position.y(), position.z() + t)
		);
		return delta * magnitude;
	}
};

#endif
