#ifndef _INTEGRATOR_H_
#define _INTEGRATOR_H_

#include "particle.h"
#include "ForceField.h"

class Integrator
{
public:
	virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) = 0;
	virtual Vec3f getColor() const = 0;
};

#endif // !_INTEGRATOR_H_
