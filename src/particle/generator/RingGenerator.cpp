#include "RingGenerator.h"
#include <GL/glut.h>

void RingGenerator::Paint() const
{
	float big = 1e4f;
	Vec3f v1 = v_x * big;
	Vec3f v2 = v_y * big;
	glPushMatrix();
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
		glVertex3f(v1.x(), v1.y(), v1.z());
		glVertex3f(v2.x(), v2.y(), v2.z());
		glVertex3f(-v1.x(), -v1.y(), -v1.z());
		glVertex3f(-v2.x(), -v2.y(), -v2.z());
	glEnd();
	glPopMatrix();
}
