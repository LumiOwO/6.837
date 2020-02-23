#ifndef PLANE_H
#define PLANE_H

#include "object3d.h"

class Plane : public Object3D
{
public:
	Plane(Vec3f normal, float offset, Material* material);
	virtual ~Plane() {}

	virtual Bound objBound() const override;
	virtual Bound worldBound() const override;
	virtual bool intersect(const Ray &r, Hit &h, float tmin) const override;
	virtual void paint() const override;
	virtual void insertIntoGrid(Grid *grid) const override;
};

#endif // PLANE_H
