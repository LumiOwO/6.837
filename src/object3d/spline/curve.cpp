#include "curve.h"
#include <GL/glut.h>

void Curve::Paint(ArgParser *args) {
	// draw lines
	glLineWidth(1);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for (Vec3f p : v) {
		glVertex3f(p.x(), p.y(), p.z());
	}
	glEnd();

	// draw curve
	int num = args->curve_tessellation;
	float delta = 1.0f / num;
	int units = numOfUnits();
	glLineWidth(3);
	glColor3f(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < units; i++) {
		float t = 0;
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= num; j++) {
			Vec3f p = Q(t, i);
			glVertex3f(p.x(), p.y(), p.z());
			t += delta;
		}
		glEnd();
	}

	// draw points
	glPointSize(6);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	for (Vec3f p : v) {
		glVertex3f(p.x(), p.y(), p.z());
	}
	glEnd();

}

Vec3f Curve::Q(float t, int unit) const
{
	vector<Vec3f> cp = unitCP(unit);
	Matrix G{
		cp[0].x(),cp[1].x(),cp[2].x(),cp[3].x(),
		cp[0].y(),cp[1].y(),cp[2].y(),cp[3].y(),
		cp[0].z(),cp[1].z(),cp[2].z(),cp[3].z(),
		0, 0, 0, 0,
	};
	Vec4f T(t*t*t, t*t, t, 1);

	return G * B() * T;
}
