#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.h"

class Triangle: public Object3D
{
private:
	Vec3f A;
	Vec3f B;
	Vec3f C;

	Vec3f normal;
public:
	Triangle(Vec3f v0, Vec3f v1, Vec3f v2, Material* current_material);
	virtual ~Triangle() {}

	virtual Bound objBound() const override;
	virtual Bound worldBound() const override;
	virtual bool intersect(const Ray &r, Hit &h, float tmin) const override;
	virtual void paint() const override;

	virtual void insertIntoGrid(Grid *grid) const override;
};

#endif // TRIANGLE_H
