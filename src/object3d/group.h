#ifndef GROUP_H
#define GROUP_H

#include "object3d.h"

class Group: public Object3D
{
private:
	Object3D **members;
	int size;
public:
	Group(int size);
	~Group();

	virtual Bound objBound() const override { return worldBound(); }
	virtual Bound worldBound() const override;
	virtual bool intersect(const Ray &r, Hit &h, float tmin) const override;
	virtual void paint() const override;

	virtual void insertIntoGrid(Grid *grid) const override;
	virtual void applyTransform(const Matrix &m, const Matrix &inv) override;

	void addObject(int index, Object3D *obj);
};

#endif // GROUP_H
