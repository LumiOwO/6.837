#ifndef _GAUSSIANFILTER_H_
#define _GAUSSIANFILTER_H_

#include "filter.h"

class GaussianFilter: public Filter
{
private:
	const float sigma;
	const float sigma2_mul4;
	const float inv_sigma2_mul2;
	const int radius;

public:
	GaussianFilter(float sigma): sigma(sigma), 
		sigma2_mul4(4.0f * sigma * sigma),
		inv_sigma2_mul2(1.0f / (2 * sigma * sigma)),
		radius(int(2 * sigma + 0.5f - 1e-5f)) { }

	virtual float getWeight(float x, float y) const {
		float d_square = x * x + y * y;
		float expo = -d_square * inv_sigma2_mul2;
		return exp(expo);
	}
	virtual int getSupportRadius() const override { return radius; }
};

#endif // !_GAUSSIANFILTER_H_
