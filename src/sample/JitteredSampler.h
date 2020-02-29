#ifndef _JITTEREDSAMPLER_H_
#define _JITTEREDSAMPLER_H_

#include "UniformSampler.h"

class JitteredSampler: public UniformSampler
{
public:
	JitteredSampler(int n) : UniformSampler(n) {}
	virtual Vec2f getSamplePosition(int i) const override {
		Vec2f ret = UniformSampler::getSamplePosition(i);
		Vec2f offset(rand_f_range(-0.5f, 0.5f), rand_f_range(-0.5f, 0.5f));
		return ret + offset * getDelta();
	}
};

#endif // !_JITTEREDSAMPLER_H_
