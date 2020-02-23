#include "group.h"
#include <cfloat>

Group::Group(int size): size(size)
{
	assert(size > 0);
	members = new Object3D*[size];
	for(int i=0; i<size; i++)
		members[i] = nullptr;
}

Group::~Group()
{
	for(int i=0; i<size; i++)
		if(members[i]) delete members[i];
	delete [] members;
}

Bound Group::worldBound() const
{
	Bound box = members[0]->worldBound();
	for (int i = 1; i < size; i++) {
		Bound curBox = members[i]->worldBound();
		if(curBox.isValid())
			box = box.Extend(curBox);
	}
	return box;
}

bool Group::intersect(const Ray &r, Hit &h, float tmin) const
{
	bool ret = false;
	float min = FLT_MAX;
	Hit buffer;
	for(int i=0; i<size; i++) {
		if(members[i]->intersect(r, buffer, tmin)) {
			ret = true;
			if(buffer.getT() < min) {
				min = buffer.getT();
				h = buffer;
			}
		}
	}
	return ret;
}

void Group::paint() const
{
	for (int i = 0; i < size; i++) {
		members[i]->paint();
	}
}

void Group::insertIntoGrid(Grid *grid) const
{
	for (int i = 0; i < size; i++) {
		members[i]->insertIntoGrid(grid);
	}
}

void Group::applyTransform(const Matrix &m, const Matrix &inv)
{
	for (int i = 0; i < size; i++) {
		members[i]->applyTransform(m, inv);
	}
}

void Group::addObject(int index, Object3D *obj)
{
	members[index] = obj;
}
