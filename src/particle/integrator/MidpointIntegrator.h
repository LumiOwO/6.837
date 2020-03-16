#ifndef _MIDPOINTINTEGRATOR_H_
#define _MIDPOINTINTEGRATOR_H_

#include "Integrator.h"

class MidpointIntegrator: public Integrator
{
public:
	virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) override {
		float m = particle->getMass();
		Vec3f p = particle->getPosition();
		Vec3f v = particle->getVelocity();
		Vec3f a = forcefield->getAcceleration(p, m, t);

		float dt_2 = dt / 2;
		Vec3f p_m = p + v * dt_2;
		Vec3f v_m = v + a * dt_2;
		Vec3f a_m = forcefield->getAcceleration(p_m, m, t + dt_2);
		Vec3f p_next = p + v_m * dt;
		Vec3f v_next = v + a_m * dt;

		particle->setPosition(p_next);
		particle->setVelocity(v_next);
		particle->increaseAge(dt);
	}

	virtual Vec3f getColor() const override {
		return GREEN;
	}
};

#endif // !_MIDPOINTINTEGRATOR_H_
