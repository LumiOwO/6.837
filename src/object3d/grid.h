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
	int nx;
	int ny;
	int nz;

	Vec3f delta;

	Voxel *** voxels;
	Voxel extras;
public:
	Grid(Bound *bb, int nx, int ny, int nz);
	virtual ~Grid();

	// setters and getters
	int getNx() const { return nx; }
	int getNy() const { return ny; }
	int getNz() const { return nz; }
	Vec3f getDelta() const { return delta; }

	void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;

	virtual Bound objBound() const override { return *bound; }
	virtual Bound worldBound() const override { return *bound; }
	virtual bool intersect(const Ray &r, Hit &h, float tmin) const override;
	virtual void paint() const override;

	Bound getBoundAt(int i, int j, int k) const;
	std::vector<int> toGridIndex(const Vec3f& position) const;
	Vec3f toPosition(const std::vector<int>& index) const;
	void addObjectTo(const Object3D* obj, int i, int j, int k) { voxels[i][j][k].push_back(obj); }
	void addObject(const Object3D* obj) { extras.push_back(obj); }
	Voxel getExtras() const { return extras; }
	Voxel getVoxelAt(int i, int j, int k) const { return voxels[i][j][k]; }
	Voxel getVoxelAt(std::vector<int> index) const { return getVoxelAt(index[0], index[1], index[2]); }
	bool cover(std::vector<int> index) const;
	Material* getMaterial(int step) const;

private:
	void initGridMaterials();
	void paintVoxelAt(int i, int j, int k) const;
};

#endif // GRID_H
