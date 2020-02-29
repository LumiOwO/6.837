#ifndef _RAMDOMSAMPLER_H_
#define _RAMDOMSAMPLER_H_

#include "sampler.h"

class RandomSampler: public Sampler
{
public:
	RandomSampler(int n): Sampler(n) {}
	virtual Vec2f getSamplePosition(int i) const override {
		return Vec2f(rand_f(), rand_f());
	}
};

#endif

