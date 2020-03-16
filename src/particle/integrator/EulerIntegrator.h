#ifndef _EULERINTEGRATOR_H_
#define _EULERINTEGRATOR_H_

#include "Integrator.h"

class EulerIntegrator: public Integrator
{
public:
	virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) override {
		float m = particle->getMass();
		Vec3f p = particle->getPosition();
		Vec3f v = particle->getVelocity();
		Vec3f a = forcefield->getAcceleration(p, m, t);

		Vec3f p_next = p + v * dt;
		Vec3f v_next = v + a * dt;

		particle->setPosition(p_next);
		particle->setVelocity(v_next);
		particle->increaseAge(dt);
	}

	virtual Vec3f getColor() const override {
		return RED;
	}
};

#endif // !_EULERINTEGRATOR_H_
