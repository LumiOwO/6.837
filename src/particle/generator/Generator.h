#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "particle.h"
#include "random.h"

class Generator
{
protected:
	Vec3f p;
	float p_rand;
	Vec3f v;
	float v_rand;

	Vec3f color;
	Vec3f dead_color;
	float color_rand;

	float lifespan;
	float lifespan_rand;
	int desired_num;

	float m;
	float m_rand;

	Random *rand;
	const int SEED = 2020;
public:
	Generator(const Vec3f &position, float position_randomness,
		const Vec3f &velocity, float velocity_randomness) :
		p(position), p_rand(position_randomness),
		v(velocity), v_rand(velocity_randomness),
		rand(new Random(SEED)), color_rand(0), 
		lifespan(1.f), m(1), desired_num(0),
		lifespan_rand(0), m_rand(0) { }
	virtual ~Generator() { delete rand; }

	// initialization
	void SetColors(Vec3f color, Vec3f dead_color, float color_randomness) {
		this->color = color;
		this->dead_color = dead_color;
		this->color_rand = color_randomness;
	}
	void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles) {
		this->lifespan = lifespan;
		this->lifespan_rand = lifespan_randomness;
		this->desired_num = desired_num_particles;
	}
	void SetMass(float mass, float mass_randomness) {
		this->m = mass;
		this->m_rand = mass_randomness;
	}

	// on each timestep, create some particles
	virtual int numNewParticles(float current_time, float dt) const = 0;
	virtual Particle* Generate(float current_time, int i) = 0;

	// for the gui
	virtual void Paint() const = 0;
	void Restart() {
		delete rand;
		rand = new Random(SEED);
	}
};

#endif // !_GENERATOR_H_