#include "grid.h"
#include <cstring>
#include <GL/glut.h>

#include "marchinginfo.h"
#include "rayTree.h"
#include "phongmaterial.h"
#include "raytracing_stats.h"

#define WHITE	Vec3f(1, 1, 1)
#define RED		Vec3f(1, 0, 0)
#define ORANGE	Vec3f(1, 0.5, 0)
#define YELLOW	Vec3f(1, 1, 0)
#define GREEN	Vec3f(0, 1, 0)
#define CYAN	Vec3f(0, 1, 1)
#define BLUE	Vec3f(0, 0, 1)
#define MAGENTA	Vec3f(1, 0, 1)

Grid::Grid(Bound *bb, int nx, int ny, int nz) :
	bound(bb), nx(nx), ny(ny), nz(nz),
	delta(bound->diagonal(nx, ny, nz))
{
	voxels = new Voxel** [nx];
	for(int i=0; i<nx; i++) {
		voxels[i] = new Voxel* [ny];
		for(int j=0; j<ny; j++) {
			voxels[i][j] = new Voxel [nz];
			for(int k=0; k<nz; k++) {
				voxels[i][j][k].clear();
			}
		}
	}
	// init grid materials
	for (int i = 0; i < SIZE; i++) {
		int idx = i > 6 ? 6 : i;
		materials[i] = getMaterial(idx);
	}
}

Grid::~Grid()
{
	for(int i=0; i<nx; i++) {
		for(int j=0; j<ny; j++) {
			delete[] voxels[i][j];
		}
		delete[] voxels[i];
	}
	delete[] voxels;

	for(int i=0; i<SIZE; i++)
		delete materials[i];
}

void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const
{
	Hit h;
	mi.hasHit = bound->intersect(r, h, tmin);
	if (!mi.hasHit)
		return;

	Vec3f o = r.getOrigin();
	Vec3f d = r.getDirection();
	Vec3f min = bound->getMin();

	mi.t = h.getT();
	mi.normal = h.getNormal();
	for (int i = 0; i < 3; i++) {
		// init deltaT
		mi.deltaT[i] = delta[i] / std::abs(d[i]);
		// init index
		mi.index[i] = toIndex(h.getIntersectionPoint(), i);
		// init sign
		mi.sign[i] = d[i] > 0 ? 1 : -1;
		// init nextT
		if (d[i] == 0) {
			mi.nextT[i] = FLT_MAX;
		} else if (d[i] < 0) {
			mi.nextT[i] = (mi.index[i] * delta[i] - (o[i] - min[i]) ) / d[i];
		} else {
			mi.nextT[i] = ((mi.index[i] + 1) * delta[i] - (o[i] - min[i]) ) / d[i];
		}
	}
	// ensure the index is in the grid
	if (!inGrid(mi.index))
		mi.nextCell();
	// if the ray origin is in the grid, then the hitpoint t is negetive
	// so we need to traversal to the front of the origin
	while(mi.nextT[0] < 0 || mi.nextT[1] < 0 || mi.nextT[2] < 0)
		mi.nextCell();
}

bool Grid::intersect(const Ray &r, Hit &h, float tmin) const
{
	RayTracingStats::IncrementNumIntersections();

	MarchingInfo info;
	initializeRayMarch(info, r, tmin);
	bool ret = false;

	int step = 0;
	while(info.hasHit && inGrid(info.index)) {
		Bound box = getBoundAt(info.index);
		Vec3f min = box.getMin();
		Vec3f max = box.getMax();

		Vec3f vertices[8] = {
			Vec3f( min.x(), min.y(), min.z() ),
			Vec3f( max.x(), min.y(), min.z() ),
			Vec3f( max.x(), max.y(), min.z() ),
			Vec3f( min.x(), max.y(), min.z() ),

			Vec3f( min.x(), min.y(), max.z() ),
			Vec3f( max.x(), min.y(), max.z() ),
			Vec3f( max.x(), max.y(), max.z() ),
			Vec3f( min.x(), max.y(), max.z() ),
		};
		Vec3f faces[6][4] = {
			{ vertices[0], vertices[3], vertices[2], vertices[1] },
			{ vertices[0], vertices[1], vertices[5], vertices[4] },
			{ vertices[1], vertices[2], vertices[6], vertices[5] },
			{ vertices[2], vertices[3], vertices[7], vertices[6] },
			{ vertices[0], vertices[4], vertices[7], vertices[3] },
			{ vertices[4], vertices[5], vertices[6], vertices[7] },
		};
		Vec3f normals[6] = {
			Vec3f(  0,  0, -1 ),
			Vec3f(  0, -1,  0 ),
			Vec3f(  1,  0,  0 ),
			Vec3f(  0,  1,  0 ),
			Vec3f( -1,  0,  0 ),
			Vec3f(  0,  0,  1 ),
		};
		for(int i=0; i<6; i++) {
			RayTree::AddHitCellFace(
				faces[i][0], faces[i][1], faces[i][2], faces[i][3],
				normals[i], getMaterial(step)
			);
			if (info.normal == normals[i]) {
				RayTree::AddEnteredFace(
					faces[i][0], faces[i][1], faces[i][2], faces[i][3],
					normals[i], getMaterial(step)
				);
			}
		}

		int size = (int)voxels[info.index[0]][info.index[1]][info.index[2]].size();
		if(!ret && size > 0) {
			h.set(info.t, materials[size - 1], info.normal, r);
			ret = true;
//			break;
		}

		// to next cell
		info.nextCell();
		RayTracingStats::IncrementNumGridCellsTraversed();
		step ++;
	}
	return ret;
}

void Grid::paint() const
{
	glPushMatrix();
	glBegin(GL_QUADS);

	for(int i=0; i<nx; i++)
	for(int j=0; j<ny; j++)
	for(int k=0; k<nz; k++)
		if(voxels[i][j][k].size() > 0)
			paintVoxelAt(i, j, k);

	glEnd();
	glPopMatrix();
}

Bound Grid::getBoundAt(int i, int j, int k) const
{
	Vec3f min = bound->getMin() + delta * Vec3f((float)i, (float)j, (float)k);
	Vec3f max = min + delta;
	return Bound(min, max);
}

void Grid::paintVoxelAt(int i, int j, int k) const
{
	Bound box = getBoundAt(i, j, k);
	Vec3f min = box.getMin();
	Vec3f max = box.getMax();

	float vertices[8][3] = {
		{ min.x(), min.y(), min.z() },
		{ max.x(), min.y(), min.z() },
		{ max.x(), max.y(), min.z() },
		{ min.x(), max.y(), min.z() },

		{ min.x(), min.y(), max.z() },
		{ max.x(), min.y(), max.z() },
		{ max.x(), max.y(), max.z() },
		{ min.x(), max.y(), max.z() },
	};

	float* faces[6][4] = {
		{ vertices[0], vertices[3], vertices[2], vertices[1] },
		{ vertices[0], vertices[1], vertices[5], vertices[4] },
		{ vertices[1], vertices[2], vertices[6], vertices[5] },
		{ vertices[2], vertices[3], vertices[7], vertices[6] },
		{ vertices[0], vertices[4], vertices[7], vertices[3] },
		{ vertices[4], vertices[5], vertices[6], vertices[7] },
	};

	float normals[6][3] = {
		{  0,  0, -1 },
		{  0, -1,  0 },
		{  1,  0,  0 },
		{  0,  1,  0 },
		{ -1,  0,  0 },
		{  0,  0,  1 },
	};

	bool isInside[6] = {
		k > 0    && voxels[i][j][k-1].size() > 0,
		j > 0    && voxels[i][j-1][k].size() > 0,
		i < nx-1 && voxels[i+1][j][k].size() > 0,
		j < ny-1 && voxels[i][j+1][k].size() > 0,
		i > 0    && voxels[i-1][j][k].size() > 0,
		k < nz-1 && voxels[i][j][k+1].size() > 0,
	};

	// draw the faces
	materials[voxels[i][j][k].size() - 1]->glSetMaterial();
	for(int i=0; i<6; i++) {
		if(!isInside[i]) {
			glNormal3fv(normals[i]);
			for(int j=0; j<4; j++) {
				glVertex3fv(faces[i][j]);
			}
		}
	}
}

static Vec3f colors[] = {
	WHITE,
	MAGENTA,
	GREEN,
	BLUE,
	CYAN,
	YELLOW,
	RED,
};

Material *Grid::getMaterial(int step) const
{
	int size = sizeof(colors) / sizeof(colors[0]);
	return new PhongMaterial(
		colors[step % size],
		Vec3f(0, 0, 0),
		1,
		Vec3f(0, 0, 0),
		Vec3f(0, 0, 0),
		1
	);
}

