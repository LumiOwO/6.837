#ifndef _SAMPLER_H_
#define _SAMPLER_H_

#include "vectors.h"

class Sampler 
{
public:
	int n;

	Sampler(int n) : n(n) {};
	virtual Vec2f getSamplePosition(int i) const = 0;
};

#endif
