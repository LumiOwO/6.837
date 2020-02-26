#ifndef GRID_H
#define GRID_H

#include "object3d.h"
#include <vector>

typedef std::vector<const Object3D*> Voxel;

class MarchingInfo;

class Grid: public Object3D
{
private:
	static const int SIZE = 100;
	Material* materials[SIZE];
	Bound *bound;
	Voxel *** voxels;
	Voxel extras;
public:
	const int nx;
	const int ny;
	const int nz;
	const Vec3f delta;

public:
	Grid(Bound *bb, int nx, int ny, int nz);
	virtual ~Grid();

	virtual Bound objBound() const override { return *bound; }
	virtual Bound worldBound() const override { return *bound; }
	virtual bool intersect(const Ray &r, Hit &h, float tmin) const override;
	virtual void paint() const override;

	virtual void insertIntoGrid(Grid *grid) const override { assert(0); }
	virtual void applyTransform(const Matrix &m, const Matrix &inv) override { assert(0); }

	void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;

	// utils
	Voxel getExtras() const { return extras; }
	void addExtra(const Object3D* obj) { extras.push_back(obj); }
	Voxel getVoxelAt(int index[]) const { 
		return voxels[index[0]][index[1]][index[2]]; 
	}
	void addObjectTo(const Object3D* obj, int i, int j, int k) { 
		voxels[i][j][k].push_back(obj); 
	}
	bool inGrid(int i, int j, int k) const {
		return i >= 0 && i < nx
			&& j >= 0 && j < ny
			&& k >= 0 && k < nz;
	}
	bool inGrid(int index[]) const { 
		return inGrid(index[0], index[1], index[2]); 
	}
	int toIndex(Vec3f p, int axis) const {
		return (int)std::floor((p[axis] - bound->getMin()[axis]) / delta[axis]);
	}
	Bound getBoundAt(int i, int j, int k) const;
	Bound getBoundAt(int index[]) const {
		return getBoundAt(index[0], index[1], index[2]);
	}

private:
	void paintVoxelAt(int i, int j, int k) const;
	Material* getMaterial(int step) const;
};

#endif // GRID_H
