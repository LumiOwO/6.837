#ifndef _FORCEFIELD_H_
#define _FORCEFIELD_H_

#include "vectors.h"

class ForceField 
{
public:
	virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const = 0;
};

#endif
