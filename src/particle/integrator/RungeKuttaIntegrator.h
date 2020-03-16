#ifndef _RUNGEKUTTAINTEGRATOR_H_
#define _RUNGEKUTTAINTEGRATOR_H_

#include "Integrator.h"

class RungeKuttaIntegrator: public Integrator
{
public:
	virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) override {
		float m = particle->getMass();
		Vec3f p = particle->getPosition();
		Vec3f v = particle->getVelocity();
		Vec3f a = forcefield->getAcceleration(p, m, t);
		float dt_2 = dt / 2;

		// f1 = y' = v;
		// f2 = v' = f(t, v, y);
		// [y v]' = [f1 f2]
		Vec3f vK1 = dt * a;
		Vec3f pK1 = dt * v;
		Vec3f vK2 = dt * forcefield->getAcceleration(p + pK1 / 2, m, t + dt_2);
		Vec3f pK2 = dt * (v + vK1 / 2);
		Vec3f vK3 = dt * forcefield->getAcceleration(p + pK2 / 2, m, t + dt_2);
		Vec3f pK3 = dt * (v + vK2 / 2);
		Vec3f vK4 = dt * forcefield->getAcceleration(p + pK3, m, t + dt);
		Vec3f pK4 = dt * (v + vK3);

		Vec3f v_next = v + 1.f / 6 * (vK1 + 2 * vK2 + 2 * vK3 + vK4);
		Vec3f p_next = p + 1.f / 6 * (pK1 + 2 * pK2 + 2 * pK3 + pK4);
		particle->setPosition(p_next);
		particle->setVelocity(v_next);
		particle->increaseAge(dt);
	}

	virtual Vec3f getColor() const override {
		return BLUE;
	}
};

#endif // !_RUNGEKUTTAINTEGRATOR_H_
