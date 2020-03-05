#include "BezierCurve.h"
#include "BSplineCurve.h"

const Matrix BezierCurve::B_Bezier{
	-1, 3, -3, 1,
	3, -6, 3, 0,
	-3, 3, 0, 0,
	1, 0, 0, 0
};
const Matrix BezierCurve::B_Bezier_inv{
	0, 0, 0, 1,
	0, 0, 1.0f / 3, 1,
	0, 1.0f / 3, 2.0f / 3, 1,
	1, 1, 1, 1
};

void BezierCurve::OutputBezier(FILE *file)
{
	fprintf(file, "bezier\n");
	OutputCP(file, v);
}

void BezierCurve::OutputBSpline(FILE *file)
{
	assert(v.size() == 4);
	fprintf(file, "bspline\n");
	Matrix P = Matrix{
		v[0].x(),v[1].x(),v[2].x(),v[3].x(),
		v[0].y(),v[1].y(),v[2].y(),v[3].y(),
		v[0].z(),v[1].z(),v[2].z(),v[3].z(),
		0, 0, 0, 0
	} * B() * BSplineCurve::B_BSpline_inv;
	OutputCP(file, P.toVec3fs());
}