#ifndef MARCHINGINFO_H
#define MARCHINGINFO_H

#include "vectors.h"

class MarchingInfo
{
public:
	int index[3];
	int sign[3];

	Vec3f nextT;
	Vec3f deltaT;
	Vec3f normal;

	float t;
	bool hasHit;

	MarchingInfo(): hasHit(false), t(0) {}
	~MarchingInfo() {}

	void nextCell();
	float nextT_f() const { return min2(min2(nextT[0], nextT[1]), nextT[2]); }
};

#endif // MARCHINGINFO_H
