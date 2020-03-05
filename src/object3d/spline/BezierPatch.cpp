#include "BezierPatch.h"
#include <GL/glut.h>
#include <vector>

void BezierPatch::Paint(ArgParser *args) {
	// draw lines
	glLineWidth(1);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for (Vec3f p : v) {
		glVertex3f(p.x(), p.y(), p.z());
	}
	glEnd();

	// draw curve
	int num = args->patch_tessellation;
	float delta = 1.0f / num;
	glLineWidth(3);
	glColor3f(0.0f, 1.0f, 0.0f);
	float t = 0;
	Vec3f cp[4];
	for (int ti = 0; ti <= num; ti++, t += delta) {
		for (int i = 0; i < 4; i++) {
			cp[i] = Q(t, v + (i << 2));
		}
		float s = 0;
		glBegin(GL_LINE_STRIP);
		for (int si = 0; si <= num; si++, s += delta) {
			Vec3f p = Q(s, cp);
			glVertex3f(p.x(), p.y(), p.z());
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

TriangleMesh* BezierPatch::OutputTriangles(ArgParser *args)
{
	int num = args->patch_tessellation;
	TriangleNet *net = new TriangleNet(num, num);
	float delta = 1.0f / num;
	float t = 0;
	Vec3f cp[4];
	for (int ti = 0; ti <= num; ti++, t += delta) {
		for (int i = 0; i < 4; i++) {
			cp[i] = Q(t, v + (i << 2));
		}
		float s = 0;
		for (int si = 0; si <= num; si++, s += delta) {
			net->SetVertex(ti, si, Q(s, cp));
		}
	}
	return net;
}

Vec3f BezierPatch::Q(float t, const Vec3f *cp)
{
	vector<Vec3f> p{ cp[0], cp[1], cp[2], cp[3] };
	int size;
	while ((size = p.size()) > 1) {
		for (int i = 0; i < size - 1; i++) {
			p[i] = lerp(t, p[i], p[i + 1]);
		}
		p.pop_back();
	}
	return p[0];
}
