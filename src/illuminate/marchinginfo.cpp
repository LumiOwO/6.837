#include "marchinginfo.h"

void MarchingInfo::nextCell()
{
	if(nextT[0] <= nextT[1] && nextT[0] <= nextT[2]) {
		t = nextT[0];
		index[0] += sign[0];
		nextT += Vec3f(deltaT[0], 0, 0);
		normal = Vec3f(-1.0f * sign[0], 0, 0);

	} else if(nextT[1] <= nextT[2] && nextT[1] <= nextT[0]) {
		t = nextT[1];
		index[1] += sign[1];
		nextT += Vec3f(0, deltaT[1], 0);
		normal = Vec3f(0, -1.0f * sign[1], 0);

	} else if(nextT[2] <= nextT[0] && nextT[2] <= nextT[1]) {
		t = nextT[2];
		index[2] += sign[2];
		nextT += Vec3f(0, 0, deltaT[2]);
		normal = Vec3f(0, 0, -1.0f * sign[2]);

	}
}
