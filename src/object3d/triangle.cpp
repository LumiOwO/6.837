#include "triangle.h"
#include "grid.h"
#include "transform.h"

#include "matrix.h"
#include "raytracing_stats.h"
#include <GL/glut.h>

Triangle::Triangle(Vec3f v0, Vec3f v1, Vec3f v2, Material* current_material):
	Object3D(current_material), A(v0), B(v1), C(v2)
{
	normal = Vec3f::Cross3(C - B, A - B).Normalize();
}

Bound Triangle::objBound() const
{
	return Bound(A, B).Extend(C);
}

Bound Triangle::worldBound() const
{
	return Bound(ObjtoWorld(A), ObjtoWorld(B)).Extend(ObjtoWorld(C));
}

bool Triangle::intersect(const Ray &r, Hit &h, float tmin) const
{
	RayTracingStats::IncrementNumIntersections();

	// transform vertices into ray coordinate system
	Vec3f A_w = ObjtoWorld(A);
	Vec3f B_w = ObjtoWorld(B);
	Vec3f C_w = ObjtoWorld(C);
	// translate ray origin to (0, 0, 0)
	A_w -= r.getOrigin();
	B_w -= r.getOrigin();
	C_w -= r.getOrigin();
	// permuted so that the z dimension is the one where 
	// the absolute value of the ray’s direction is largest
	int z = r.getDirection().Abs().MaxDimension();
	int x = (z + 1) % 3;
	int y = (x + 1) % 3;
	Vec3f d = r.getDirection().Permute(x, y, z);
	A_w = A_w.Permute(x, y, z);
	B_w = B_w.Permute(x, y, z);
	C_w = C_w.Permute(x, y, z);
	// shear the x and y dimensions here
	// shear the z dimension later if the ray intersects the triangle
	float Sx = -d.x() / d.z();
	float Sy = -d.y() / d.z();
	A_w.x() += Sx * A_w.z();
	A_w.y() += Sy * A_w.z();
	B_w.x() += Sx * B_w.z();
	B_w.y() += Sy * B_w.z();
	C_w.x() += Sx * C_w.z();
	C_w.y() += Sy * C_w.z();

	// compute edge function for each edge
	float e_A = float((double)B_w.x() * (double)C_w.y() - (double)B_w.y() * (double)C_w.x());
	float e_B = float((double)C_w.x() * (double)A_w.y() - (double)C_w.y() * (double)A_w.x());
	float e_C = float((double)A_w.x() * (double)B_w.y() - (double)A_w.y() * (double)B_w.x());
	// test intersection
	if ((e_A < 0 || e_B < 0 || e_C < 0) && (e_A > 0 || e_B > 0 || e_C > 0))
		return false;
	float det = e_A + e_B + e_C;
	if (det == 0)
		return false;
	// compute t value
	float Sz = 1.0f / d.z();
	A_w.z() *= Sz;
	B_w.z() *= Sz;
	C_w.z() *= Sz;
	float t = (e_A * A_w.z() + e_B * B_w.z() + e_C * C_w.z()) / det;
	if (t <= tmin)
		return false;
	// set hit info
	h.set(t, material, ObjtoWorld.TransformNormal(normal, &WorldtoObj), r);
	return true;
}

void Triangle::paint() const
{
	Vec3f n_w = ObjtoWorld.TransformNormal(normal, &WorldtoObj);
	Vec3f A_w = ObjtoWorld(A);
	Vec3f B_w = ObjtoWorld(B);
	Vec3f C_w = ObjtoWorld(C);

	material->glSetMaterial();
	glPushMatrix();
	glBegin(GL_TRIANGLES);
		glNormal3f(n_w.x(), n_w.y(), n_w.z());
		glVertex3f(A_w.x(), A_w.y(), A_w.z());
		glVertex3f(B_w.x(), B_w.y(), B_w.z());
		glVertex3f(C_w.x(), C_w.y(), C_w.z());
	glEnd();
	glPopMatrix();
}
