#include "bound.h"
#include <GL/glut.h>

#include "ray.h"
#include "raytracing_stats.h"

#include "matrix.h"
#include "hit.h"

// ====================================================================
// ====================================================================

void Bound::paint() const {
  // draw a wireframe box to represent the boundingbox
  glColor3f(1,1,1);
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);

  glVertex3f(min.x(),min.y(),min.z());
  glVertex3f(max.x(),min.y(),min.z());
  glVertex3f(min.x(),min.y(),min.z());
  glVertex3f(min.x(),max.y(),min.z());
  glVertex3f(max.x(),max.y(),min.z());
  glVertex3f(max.x(),min.y(),min.z());
  glVertex3f(max.x(),max.y(),min.z());
  glVertex3f(min.x(),max.y(),min.z());

  glVertex3f(min.x(),min.y(),min.z());
  glVertex3f(min.x(),min.y(),max.z());
  glVertex3f(min.x(),max.y(),min.z());
  glVertex3f(min.x(),max.y(),max.z());
  glVertex3f(max.x(),min.y(),min.z());
  glVertex3f(max.x(),min.y(),max.z());
  glVertex3f(max.x(),max.y(),min.z());
  glVertex3f(max.x(),max.y(),max.z());

  glVertex3f(min.x(),min.y(),max.z());
  glVertex3f(max.x(),min.y(),max.z());
  glVertex3f(min.x(),min.y(),max.z());
  glVertex3f(min.x(),max.y(),max.z());
  glVertex3f(max.x(),max.y(),max.z());
  glVertex3f(max.x(),min.y(),max.z());
  glVertex3f(max.x(),max.y(),max.z());
  glVertex3f(min.x(),max.y(),max.z());

  glEnd();
  glEnable(GL_LIGHTING);	   
}

// ====================================================================
// ====================================================================

Bound::Bound(const Bound &b)
{
	min = b.min;
	max = b.max;
	valid = b.valid;
}

Bound& Bound::operator=(const Bound &b)
{
	min = b.min;
	max = b.max;
	valid = b.valid;
	return *this;
}

bool Bound::cover(Vec3f point) const
{
	return point.x() >= min.x() && point.x() <= max.x()
		&& point.y() >= min.y() && point.y() <= max.y()
		&& point.z() >= min.z() && point.z() <= max.z();
}

bool Bound::intersect(const Bound &box) const
{
	return  min.x() <= box.max.x() &&
			min.y() <= box.max.y() &&
			min.z() <= box.max.z() &&
			max.x() >= box.min.x() &&
			max.y() >= box.min.y() &&
			max.z() >= box.min.z();
}

bool Bound::intersect(const Ray &r, Hit &h, float tmin) const
{
	RayTracingStats::IncrementNumIntersections();

	Vec3f d = r.getDirection();
	Vec3f o = r.getOrigin();
	Vec3f normal;
	float hit_min = 0;
	float hit_max = 1e6;

	for(int i = 0; i < 3; i ++) {
		float inv_dir = 1 / d[i];

		float hit_near = (min[i] - o[i]) * inv_dir;
		float hit_far  = (max[i] - o[i]) * inv_dir;
		if (hit_near > hit_far) {
			std::swap(hit_near, hit_far);
		}
		// make it more robust (reference: pbr-book 3.9.2)
		hit_far *= 1 + 2 * gamma(3);

		// update hit point
		if(hit_near > hit_min) {
			hit_min = hit_near;
			normal = Vec3f(0, 0, 0);
			normal[i] = d[i] > 0 ? -1.0f : 1.0f;
		}
		hit_max = hit_far < hit_max? hit_far: hit_max;

		if (hit_min > hit_max) {
			return false;
		}
	}

	float hitT = hit_min;
	if (hit_max < tmin) {
		return false;
	}
	h.set(hitT, nullptr, normal, r);
	return true;
}

