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

Grid::Grid(Bound *bb, int nx, int ny, int nz):
	bound(bb), nx(nx), ny(ny), nz(nz)
{
	Vec3f max = bound->getMax();
	Vec3f min = bound->getMin();
	delta = Vec3f(
		(max.x() - min.x()) / nx,
		(max.y() - min.y()) / ny,
		(max.z() - min.z()) / nz
	);

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

	initGridMaterials();
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

void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float) const
{
	Vec3f origin = r.getOrigin();
	Vec3f direction = r.getDirection();

	float hitT = 0;
	Vec3f normal;
	if(bound->cover(origin) || bound->intersect(r, normal, hitT))
	{
		Vec3f hitPoint = r.pointAtParameter(hitT);
		vector<int> index = toGridIndex(hitPoint);
		Vec3f pos = toPosition(index);

		for(int i=0; i<3; i++)
		{
			mi.setIndexAt(i, index[(unsigned int)i]);
			if(fabsf(direction[i]) < 1e-5) {
				mi.setSignAt(i, 0);
				mi.setDeltaAt(i, 1e7);
				mi.setNextAt(i, 1e7);
			} else {
				float sign = direction[i] > 0? 1.0f: -1.0f;
				float curDelta = delta[i] / fabsf(direction[i]);
				mi.setSignAt(i, sign);
				mi.setDeltaAt(i, curDelta);

				float diff = (pos[i] - hitPoint[i]) / direction[i];
				float next = hitT + diff + curDelta;
				mi.setNextAt(i, next);
			}
		}
		mi.setNowT(hitT);
		mi.setNormal(normal);
		mi.setHasHit(true);
	}
}

bool Grid::intersect(const Ray &r, Hit &h, float tmin) const
{
	RayTracingStats::IncrementNumIntersections();
	MarchingInfo info;
	initializeRayMarch(info, r, tmin);

	int step = 0;
	bool ret = false;
	int i = info.getIndexAt(0);
	int j = info.getIndexAt(1);
	int k = info.getIndexAt(2);
	while(info.hasHit()
		  && i >= 0 && i <= nx
		  && j >= 0 && j <= ny
		  && k >= 0 && k <= nz)
	{
//		cout << "(" << i << ", " << j << ", " << k << ")" << endl;
		if(i == nx || j == ny || k == nz)
			;
		else {
			Bound box = getBoundAt(i, j, k);
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
				if(info.getNormal() == normals[i])
					RayTree::AddEnteredFace(
						faces[i][0], faces[i][1], faces[i][2], faces[i][3],
						normals[i], getMaterial(step)
					);
			}

			if(!ret && voxels[i][j][k].size() > 0) {
				h.set(info.getNowT(), materials[voxels[i][j][k].size() - 1], info.getNormal(), r);
				ret = true;
//				break;
			}
		}
		// to next cell
		info.nextCell();
		RayTracingStats::IncrementNumGridCellsTraversed();
		i = info.getIndexAt(0);
		j = info.getIndexAt(1);
		k = info.getIndexAt(2);
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

void Grid::initGridMaterials()
{
	for(int i=0; i<SIZE; i++) {
		int idx = i > 6? 6: i;
		materials[i] = getMaterial(idx);
	}
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

Material *Grid::getMaterial(int step) const
{
	Vec3f colors[] = {
		WHITE,
		MAGENTA,
		GREEN,
		BLUE,
		CYAN,
		YELLOW,
		RED,
	};
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

std::vector<int> Grid::toGridIndex(const Vec3f& position) const
{
	std::vector<int> ret(3);
	for(unsigned int i=0; i<3; i++)
		ret[i] = int((position[(int)i] - bound->getMin()[(int)i]) / delta[(int)i]);
	return ret;
}

Vec3f Grid::toPosition(const std::vector<int>& index) const
{
	Vec3f ret;
	for(int i=0; i<3; i++)
		ret[i] = bound->getMin()[i] + index[(unsigned int)i] * delta[i] + delta[i] / 2;
	return ret;
}

bool Grid::cover(std::vector<int> index) const
{
	return index[0] >= 0 && index[0] < nx
		&& index[1] >= 0 && index[1] < ny
		&& index[2] >= 0 && index[2] < nz;
}
