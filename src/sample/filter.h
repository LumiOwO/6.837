#ifndef _FILTER_H_
#define _FILTER_H_

#include "vectors.h"

class Film;

class Filter
{
public:
	Vec3f getColor(int i, int j, Film *film) const;

	virtual float getWeight(float x, float y) const = 0;
	virtual int getSupportRadius() const = 0;
};

#endif
