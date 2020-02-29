#ifndef _BOXFILTER_H_
#define _BOXFILTER_H_

#include "filter.h"

class BoxFilter : public Filter
{
private:
	const float radius;
public:
	BoxFilter(float r): radius(r) {}

	virtual float getWeight(float x, float y) const override { 
		return (x < radius && y < radius && x > -radius && y > -radius)? 1.0f: 0.0f;
	}
	virtual int getSupportRadius() const override { 
		return int(radius + 0.5f - 1e-5f); 
	}
};

#endif
