#ifndef _UNIFORMSAMPLER_H_
#define _UNIFORMSAMPLER_H_

#include "sampler.h"

class UniformSampler : public Sampler
{
private:
	int size;
	float delta;
public:
	UniformSampler(int n): Sampler(n), size(1) {
		assert(n > 0);
		// n is rounded up to square
		int i = 1;
		for (; size < n; i++) {
			size += (i << 1) + 1;
		}
		Sampler::n = size;
		size = i;
		delta = 1.0f / (size + 1);
	}
	virtual Vec2f getSamplePosition(int i) const override {
		return Vec2f((i / size + 1) * delta, (i % size + 1) * delta);
	}
protected:
	float getDelta() const { return delta; }
};

#endif