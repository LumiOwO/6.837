#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "object3d.h"
#include "matrix.h"

class Transform: public Object3D
{
private:
	Object3D *object;
public:
	Transform(Object3D *o, Matrix &m, Matrix &inv): object(o) {
		object->applyTransform(m, inv);
	}
	virtual ~Transform() { delete object; }

	virtual Bound objBound() const override { return object->objBound(); }
	virtual Bound worldBound() const override { return object->worldBound(); }
	virtual bool intersect(const Ray &r, Hit &h, float tmin) const override { 
		return object->intersect(r, h, tmin); 
	}
	virtual void paint() const override { object->paint(); }
	virtual void insertIntoGrid(Grid *grid) const override { object->insertIntoGrid(grid); }
	virtual void applyTransform(const Matrix &m, const Matrix &inv) override {
		object->applyTransform(m, inv);
	}
};

#endif // TRANSFORM_H
