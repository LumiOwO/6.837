#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "material.h"
#include "ray.h"
#include "hit.h"

#include "bound.h"

#include "matrix.h"

class Grid;

class Object3D
{
public:
	Material* material;
	Matrix ObjtoWorld;
	Matrix WorldtoObj;

	Object3D(Material* m = nullptr): material(m) {}
	virtual ~Object3D() {}

	virtual Bound objBound() const = 0;
	virtual Bound worldBound() const { return ObjtoWorld(objBound()); }
	virtual bool intersect(const Ray &r, Hit &h, float tmin) const = 0;
	virtual void paint() const = 0;

	virtual void insertIntoGrid(Grid *grid) const;
	virtual void applyTransform(const Matrix &m, const Matrix &inv) {
		ObjtoWorld = m * ObjtoWorld;
		WorldtoObj = WorldtoObj * inv;
	}
};

#endif // OBJECT3D_H
