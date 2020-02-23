#include "marchinginfo.h"
#include "phongmaterial.h"

vector<int>& operator +=(vector<int>& a, const Vec3f &b)
{
	a[0] += (int)b[0];
	a[1] += (int)b[1];
	a[2] += (int)b[2];
	return a;
}

void MarchingInfo::nextCell()
{
	if(next_T.x() <= next_T.y() && next_T.x() <= next_T.z()) {
		nowT = next_T.x();
		now_index += Vec3f(sign.x(), 0, 0);
		next_T += Vec3f(increment_T.x(), 0, 0);
		normal = Vec3f(-sign.x(), 0, 0);

	} else if(next_T.y() <= next_T.z() && next_T.y() <= next_T.x()) {
		nowT = next_T.y();
		now_index += Vec3f(0, sign.y(), 0);
		next_T += Vec3f(0, increment_T.y(), 0);
		normal = Vec3f(0, -sign.y(), 0);

	} else if(next_T.z() <= next_T.x() && next_T.z() <= next_T.y()) {
		nowT = next_T.z();
		now_index += Vec3f(0, 0, sign.z());
		next_T += Vec3f(0, 0, increment_T.z());
		normal = Vec3f(0, 0, -sign.z());

	}
}
