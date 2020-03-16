#ifndef _HOSEGENERATOR_H_
#define _HOSEGENERATOR_H_

#include "Generator.h"

class HoseGenerator: public Generator
{
public:
	HoseGenerator(const Vec3f &position, float position_randomness,
		const Vec3f &velocity, float velocity_randomness):
		Generator(position, position_randomness, 
				velocity, velocity_randomness) {}

	// on each timestep, create some particles
	virtual int numNewParticles(float current_time, float dt) const override {
		return (int)ceil(dt * desired_num / lifespan);
	}
	virtual Particle* Generate(float current_time, int i) override {
		Vec3f p_new = p + rand->randomVector() * p_rand;
		Vec3f v_new = v + rand->randomVector() * v_rand;
		Vec3f color_new = color + rand->randomVector() * color_rand;
		float m_new = m + rand->next() * m_rand;
		float life_new = lifespan + rand->next() * lifespan_rand;

		return new Particle(p_new, v_new, color_new, dead_color, m_new, life_new);
	}

	// for the gui
	virtual void Paint() const override {

	}
};

#endif // !_HOSEGENERATOR_H_