#include "sphere.h"
#include "grid.h"
#include "transform.h"

#include "raytracing_stats.h"
#include "glCanvas.h"
#include "matrix.h"
#include <GL/glut.h>

Sphere::Sphere(Vec3f center, float radius, Material* material):
	Object3D(material), radius(radius)
{
	ObjtoWorld = Matrix::MakeTranslation(center);
	WorldtoObj = Matrix::MakeTranslation(-center);
}

Bound Sphere::objBound() const
{
	return Bound(Vec3f(-radius, -radius, -radius),
				 Vec3f( radius,  radius,  radius));
}

bool Sphere::intersect(const Ray &r, Hit &h, float tmin) const
{
	RayTracingStats::IncrementNumIntersections();

	Ray ray = WorldtoObj(r);
	Vec3f ray_o = ray.getOrigin();
	Vec3f ray_d = ray.getDirection();
	float a = ray_d.x() * ray_d.x() + ray_d.y() * ray_d.y() + ray_d.z() * ray_d.z();
	float b = 2 * (ray_d.x() * ray_o.x() + ray_d.y() * ray_o.y() + ray_d.z() * ray_o.z());
	float c = ray_o.x() * ray_o.x() + ray_o.y() * ray_o.y() + ray_o.z() * ray_o.z() - radius * radius;
	float t0, t1;
	if (!solveQuadratic(a, b, c, t0, t1)) {
		return false;
	} else if (t1 <= tmin) {
		return false;
	} else {
		float tHit = t0 > tmin ? t0 : t1;
		Vec3f hitPoint = ray.pointAtParameter(tHit);
		Vec3f normal = ObjtoWorld.TransformNormal(hitPoint, &WorldtoObj);
		h.set(tHit, material, normal, r);
	}
	return true;
}

void Sphere::paint() const
{
	float delta_phi_rad = (float)M_PI / GLCanvas::phi_steps;
	float delta_theta_rad = (float)M_PI * 2 / GLCanvas::theta_steps;
	Matrix phi_rotate = Matrix::MakeXRotation(delta_phi_rad);
	Matrix theta_rotate = Matrix::MakeYRotation(delta_theta_rad);

	// allocate space to store vector
	Vec3f ***pVectors = new Vec3f**[GLCanvas::phi_steps + 1];
	for(int i=0; i<GLCanvas::phi_steps + 1; i++) {
		pVectors[i] = new Vec3f*[GLCanvas::theta_steps];
		memset(pVectors[i], 0, sizeof(Vec3f*) * (unsigned int)GLCanvas::theta_steps);
	}
	pVectors[0][0] = new Vec3f(0, -radius, 0);

	// begin paint
	material->glSetMaterial();
	glPushMatrix();
	for(int i=0; i<GLCanvas::phi_steps; i++)
	for(int j=0; j<GLCanvas::theta_steps; j++)
	{
	#define NEW_VEC(v, M) (new Vec3f( (M)(v) ))
		// compute vectors to the vertices
		int i_next = (i + 1) % (GLCanvas::phi_steps + 1);
		int j_next = (j + 1) % GLCanvas::theta_steps;
		if (!pVectors[i_next][j])
			pVectors[i_next][j] = NEW_VEC(*pVectors[i][j], phi_rotate);
		if(!pVectors[i][j_next])
			pVectors[i][j_next] = NEW_VEC(*pVectors[i][j], theta_rotate);
		if(!pVectors[i_next][j_next])
			pVectors[i_next][j_next] = NEW_VEC(*pVectors[i_next][j], theta_rotate);
	#undef NEW_VEC

		// compute vertices and normal
		Vec3f V[4] = {
			*pVectors[i][j],
			*pVectors[i][j_next],
			*pVectors[i_next][j_next],
			*pVectors[i_next][j]
		};
		// draw rectangle
		glBegin(GL_QUADS);
			if (!GLCanvas::gouraud) {
				Vec3f normal = ObjtoWorld.TransformNormal(
					((V[0] + V[1] + V[2] + V[3]) / 4), &WorldtoObj);
				glNormal3f(normal.x(), normal.y(), normal.z());
			}
			for (int k = 0; k < 4; k ++) {
				if (GLCanvas::gouraud) {
					Vec3f normal = ObjtoWorld.TransformNormal(
						V[k], &WorldtoObj);
					glNormal3f(normal.x(), normal.y(), normal.z());
				}
				glVertex3f(V[k].x(), V[k].y(), V[k].z());
			}
		glEnd();
	}
	glPopMatrix();

	// free memory
	for(int i=0; i<GLCanvas::phi_steps + 1; i++) {
		for(int j=0; j<GLCanvas::theta_steps; j++) {
			delete pVectors[i][j];
		}
		delete[] pVectors[i];
	}
	delete[] pVectors;
}

void Sphere::insertIntoGrid(Grid *grid) const
{
	if (!global.visualize_grid) {
		Object3D::insertIntoGrid(grid);
		return;
	}
	int minVoxel[3];
	int maxVoxel[3];
	int n[3] = { grid->nx, grid->ny, grid->nz };
	for (int i = 0; i < 3; i++) {
		minVoxel[i] = grid->toIndex(worldBound().getMin(), i);
		maxVoxel[i] = grid->toIndex(worldBound().getMax(), i);
		if (minVoxel[i] < 0) minVoxel[i] = 0;
		if (minVoxel[i] >= n[i]) minVoxel[i] = n[i] - 1;
		if (maxVoxel[i] < 0) maxVoxel[i] = 0;
		if (maxVoxel[i] >= n[i]) maxVoxel[i] = n[i] - 1;
	}
	for (int i = minVoxel[0]; i <= maxVoxel[0]; i++)
	for (int j = minVoxel[1]; j <= maxVoxel[1]; j++)
	for (int k = minVoxel[2]; k <= maxVoxel[2]; k++) {
		Vec3f mid = grid->getBoundAt(i, j, k).center();
		Vec3f vertex = mid + grid->delta / 2;
		mid = WorldtoObj(mid);
		vertex = WorldtoObj(vertex);

		if (mid.Length() <= radius + (mid - vertex).Length()) {
			grid->addObjectTo(this, i, j, k);
		}
	}
}

bool Sphere::solveQuadratic(float a, float b, float c, float &t0, float &t1) const
{
	double delta = (double)b * (double)b - 4.0 * (double)a * (double)c;
	if (delta < 0) return false;
	double rootDelta = sqrt(delta);
	t0 = float(0.5 * (-(double)b - rootDelta) / (double)a);
	t1 = float(0.5 * (-(double)b + rootDelta) / (double)a);
	if (t0 > t1) swap(t0, t1);
	return true;
}
