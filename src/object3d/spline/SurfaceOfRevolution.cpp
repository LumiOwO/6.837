#include "SurfaceOfRevolution.h"

TriangleMesh* SurfaceOfRevolution::OutputTriangles(ArgParser *args)
{
	int units = c->numOfUnits();
	int cn = args->curve_tessellation;
	int rn = args->revolution_tessellation;
	TriangleNet *net = new TriangleNet(rn, cn * units);
	float delta_c = 1.0f / cn;
	float delta_r = float(2 * M_PI / rn);

	float theta = 0;
	for (int ri = 0; ri <= rn; ri++, theta += delta_r) {
		Matrix rotate = Matrix::MakeYRotation(theta);
		for (int u = 0; u < units; u++) {
			float t = 0;
			int base = u * cn;
			for (int ci = 0; ci < cn; ci++, t += delta_c) {
				Vec3f p = c->Q(t, u);
				net->SetVertex(ri, base + ci, rotate(p));
			}
		}
		// last one
		Vec3f p = c->Q(1.0f, units - 1);
		net->SetVertex(ri, cn * units, rotate(p));
	}
	return net;
}
