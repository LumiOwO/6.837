#ifndef _RINGGENERATOR_H_
#define _RINGGENERATOR_H_

#include "Generator.h"

class RingGenerator: public Generator
{
private:
	Vec3f v_x;
	Vec3f v_y;
public:
	RingGenerator(float position_randomness,
		const Vec3f &velocity, float velocity_randomness):
		Generator(Vec3f(0, 0, 0), position_randomness,
		velocity, velocity_randomness) {
		if (velocity.x() != 0 && velocity.y() != 0) {
			v_x = Vec3f(-velocity.y(), velocity.x(), 0).Normalize();
		} else {
			v_x = Vec3f(0, velocity.z(), -velocity.y()).Normalize();
		}
		v_y = Vec3f::Cross3(v_x, velocity).Normalize();
	}

	// on each timestep, create some particles
	virtual int numNewParticles(float current_time, float dt) const override {
		return (int)ceil(dt * desired_num / lifespan);
	}
	virtual Particle* Generate(float current_time, int i) override {
		float r = current_time - int(current_time) + 0.5f;
		float k = r + (1.f - r) * rand->next();
		Vec3f p_new = p + rand->randomVector().Normalize() * k * p_rand;
		Vec3f v_new = v + rand->randomVector() * v_rand;
		Vec3f color_new = color + rand->randomVector() * color_rand;
		float m_new = m + rand->next() * m_rand;
		float life_new = lifespan + rand->next() * lifespan_rand;

		return new Particle(p_new, v_new, color_new, dead_color, m_new, life_new);
	}

	// for the gui
	virtual void Paint() const override;
};

#endif // !_RINGGENERATOR_H_