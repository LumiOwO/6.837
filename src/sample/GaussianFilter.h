#ifndef _GAUSSIANFILTER_H_
#define _GAUSSIANFILTER_H_

#include "filter.h"

class GaussianFilter: public Filter
{
private:
	const float sigma;
public:
	GaussianFilter(float sigma): sigma(sigma) {}

	virtual float getWeight(float x, float y) const {
		float d_square = x * x + y * y;
		float sigma_square = sigma * sigma;
		float expo = (-d_square) / (2 * sigma_square);
		return exp(expo);
	}
	virtual int getSupportRadius() const override { return int(2 * sigma + 0.5f - 1e-5f); }
};

#endif // !_GAUSSIANFILTER_H_
