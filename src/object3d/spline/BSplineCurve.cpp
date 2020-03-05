#include "BSplineCurve.h"
#include "BezierCurve.h"

const Matrix BSplineCurve::B_BSpline = 1.0f / 6 * Matrix{
	-1, 3, -3, 1,
	3, -6, 0, 4,
	-3, 3, 3, 1,
	1, 0, 0, 0
};
const Matrix BSplineCurve::B_BSpline_inv{
	0, 0, 0, 6,
	2.0f / 3, -1.0f / 3, 2.0f / 3, 3 + 2.0f / 3,
	-1, 0, 1, 2,
	1, 1, 1, 1
};

void BSplineCurve::OutputBezier(FILE *file)
{
	assert(v.size() == 4);
	fprintf(file, "bezier\n");
	Matrix P = Matrix{
		v[0].x(),v[1].x(),v[2].x(),v[3].x(),
		v[0].y(),v[1].y(),v[2].y(),v[3].y(),
		v[0].z(),v[1].z(),v[2].z(),v[3].z(),
		0, 0, 0, 0
	} * B() * BezierCurve::B_Bezier_inv;
	OutputCP(file, P.toVec3fs());
}

void BSplineCurve::OutputBSpline(FILE *file)
{
	fprintf(file, "bspline\n");
	OutputCP(file, v);
}