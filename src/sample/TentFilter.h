#ifndef _TENTFILTER_H_
#define _TENTFILTER_H_

#include "filter.h"

class TentFilter: public Filter
{
private:
	const float radius;
public:
	TentFilter(float r): radius(r) {}

	virtual float getWeight(float x, float y) const {
		float k = -1.0f / radius;
		float d = sqrt(x * x + y * y);
		return max2(1.0f + k * d, 0);
	}
	virtual int getSupportRadius() const override { return int(radius + 0.5f - 1e-5f); }
};

#endif // !_TENTFILTER_H_
