#ifndef MARCHINGINFO_H
#define MARCHINGINFO_H

#include "vectors.h"
#include <vector>

class MarchingInfo
{
private:
	std::vector<int> now_index;
	Vec3f next_T;
	float nowT;

	Vec3f increment_T;
	Vec3f sign;

	Vec3f normal;
	bool hit;
public:
	MarchingInfo() { now_index = std::vector<int>(3); hit = false; }
	~MarchingInfo() {}

	// setters
	void setSignAt(int axis, float value)  { sign[axis] = value; }
	void setDeltaAt(int axis, float value) { increment_T[axis] = value; }
	void setIndexAt(int axis, int value)   { now_index[(unsigned int)axis] = value; }
	void setNextAt(int axis, float value)  { next_T[axis] = value;}
	void setNowT(float value)			   { nowT = value; }
	void setNormal(const Vec3f& value)	   { normal = value; }
	void setHasHit(bool value)			   { hit = value; }

	// getter
	int getIndexAt(int axis) const  { return now_index[(unsigned int)axis]; }
	float getNowT() const			{ return nowT; }
	Vec3f getNormal() const			{ return normal; }
	bool hasHit() const				{ return hit; }

	void nextCell();
};

#endif // MARCHINGINFO_H
