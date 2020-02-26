#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.h"

class Sphere: public Object3D
{
private:
	float radius;
public:
	Sphere(Vec3f center, float radius, Material* material);
	virtual ~Sphere() {}

	virtual Bound objBound() const override;
	virtual bool intersect(const Ray &r, Hit &h, float tmin) const override;
	virtual void paint() const override;

	virtual void insertIntoGrid(Grid *grid) const;

private:
	bool solveQuadratic(float a, float b, float c, float &t0, float &t1) const;
};

#endif // SPHERE_H
