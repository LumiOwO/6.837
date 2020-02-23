#include "plane.h"
#include <GL/glut.h>

#include "grid.h"
#include "transform.h"
#include "raytracing_stats.h"

Plane::Plane(Vec3f normal, float offset, Material* material):
	Object3D(material)
{
	normal = normal.Normalize();
	// translate to origin
	if (offset > 1e-5f) {
		Vec3f t = normal * offset;
		WorldtoObj = Matrix::MakeTranslation(t) * WorldtoObj;
		ObjtoWorld = ObjtoWorld * Matrix::MakeTranslation(-t);
	}
	// permuted so that the z dimension is the one where 
	// the absolute value of the ray’s direction is largest
	int z = normal.Abs().MaxDimension();
	int x = (z + 1) % 3;
	int y = (x + 1) % 3;
	Vec3f n = normal.Permute(x, y, z);
	if (x != 0) {
		int inv[][3] = {
			{0, 1, 2}, // 0 1 2 -> 0 1 2, inv 0 1 2
			{2, 0, 1}, // 0 1 2 -> 1 2 0, inv 2 0 1
			{1, 2, 0}, // 0 1 2 -> 2 0 1, inv 1 2 0
		};
		WorldtoObj = Matrix::MakePermute(x, y, z) * WorldtoObj;
		ObjtoWorld = ObjtoWorld * Matrix::MakePermute(inv[x][0], inv[x][1], inv[x][2]);
	}
	// shear to (0, 0, 1)
	float Sx = -n.x() / n.z();
	float Sy = -n.y() / n.z();
	float Sz = 1.0f / n.z();
	float s[] = {
		1, 0, Sx, 0,
		0, 1, Sy, 0,
		0, 0, Sz, 0,
		0, 0, 0, 1
	};
	Matrix shear(s);
	WorldtoObj = shear * WorldtoObj;
	ObjtoWorld = ObjtoWorld * shear.Inverse();
}

Bound Plane::objBound() const
{
	return Bound::invalid();
}

Bound Plane::worldBound() const
{
	return Bound::invalid();
}

bool Plane::intersect(const Ray &r, Hit &h, float tmin) const
{
	RayTracingStats::IncrementNumIntersections();
	bool ret = false;

	Ray ray = WorldtoObj(r);
	if (ray.getDirection().Abs().z() < 1e-8f)
		return false;
	float t = -ray.getOrigin().z() / ray.getDirection().z();
	if (t <= tmin)
		return false;
	h.set(t, material, ObjtoWorld.TransformNormal(Vec3f(0, 0, 1), &WorldtoObj), r);
	return true;
}

void Plane::paint() const
{
	float big = 9.0f * 1e5f;
	Vec3f A = ObjtoWorld(Vec3f(-big, -big, 0));
	Vec3f B = ObjtoWorld(Vec3f(-big,  big, 0));
	Vec3f C = ObjtoWorld(Vec3f( big, -big, 0));
	Vec3f D = ObjtoWorld(Vec3f( big,  big, 0));
	Vec3f normal = ObjtoWorld.TransformNormal(Vec3f(0, 0, 1), &WorldtoObj);

	// paint
	material->glSetMaterial();
	glPushMatrix();
	glBegin(GL_QUADS);
		glNormal3f(normal.x(), normal.y(), normal.z());
		glVertex3f(A.x(), A.y(), A.z());
		glVertex3f(B.x(), B.y(), B.z());
		glVertex3f(C.x(), C.y(), C.z());
		glVertex3f(D.x(), D.y(), D.z());
	glEnd();
	glPopMatrix();
}

void Plane::insertIntoGrid(Grid *grid) const
{
	grid->addObject(this);
}
