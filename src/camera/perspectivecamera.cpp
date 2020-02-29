#include "perspectivecamera.h"
#include "matrix.h"
#include <GL/glut.h>

PerspectiveCamera::PerspectiveCamera(Vec3f center, Vec3f direction, Vec3f up, float angle_radians):
	center(center), direction(direction.Normalize()),
	up(up.Normalize()), angle_radians(angle_radians)
{
	horizontal = Vec3f::Cross3(direction, up).Normalize();
}

Ray PerspectiveCamera::generateRay(Vec2f point) const
{
	// suppose the distance is 1
	// compute the size of the plane
	Vec3f planeCenter = center + direction;
	float tan_alpha_half = tanf(angle_radians / 2);
	float planeSize = 2.0f / sqrtf(1.0f / (tan_alpha_half * tan_alpha_half) - 1.0f);

	// compute the position of the interpolate
	float x, y;
	point.Get(x, y);

	Vec3f interPoint = planeCenter +
				(x - 0.5f) * planeSize * horizontal +
				(y - 0.5f) * planeSize * up;
	Vec3f dir = (interPoint - center).Normalize();

	return Ray(center, dir);
}

// ====================================================================
// Create a perspective camera with the appropriate dimensions that
// crops or stretches in the x-dimension as necessary
// ====================================================================

void PerspectiveCamera::glInit(int w, int h) const
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		(double)angle_radians*180.0/3.14159, 1.0*w/h,
		0.5, 40.0
	);
}

// ====================================================================
// Place a perspective camera within an OpenGL scene
// ====================================================================

void PerspectiveCamera::glPlaceCamera(void) const
{
	gluLookAt(
		double(center.x()), double(center.y()), double(center.z()),
		double(center.x()+direction.x()), double(center.y()+direction.y()), double(center.z()+direction.z()),
		double(up.x()), double(up.y()), double(up.z())
	);
}

// ====================================================================
// dollyCamera, truckCamera, and RotateCamera
//
// Asumptions:
//  - up is really up (i.e., it hasn't been changed
//    to point to "screen up")
//  - up and direction are normalized
// Special considerations:
//  - If your constructor precomputes any vectors for
//    use in 'generateRay', you will likely to recompute those
//    values at the end of the these three routines
// ====================================================================

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void PerspectiveCamera::dollyCamera(float dist)
{
	center += direction * dist;

	// ===========================================
	// ASSIGNMENT 3: Fix any other affected values
	// ===========================================


}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void PerspectiveCamera::truckCamera(float dx, float dy)
{
	Vec3f screenUp;
	Vec3f::Cross3(screenUp, horizontal, direction);

	center += horizontal*dx + screenUp*dy;

	// ===========================================
	// ASSIGNMENT 3: Fix any other affected values
	// ===========================================


}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void PerspectiveCamera::rotateCamera(float rx, float ry)
{
	// Don't let the model flip upside-down (There is a singularity
	// at the poles when 'up' and 'direction' are aligned)
	float tiltAngle = acosf(up.Dot3(direction));
	if (tiltAngle-ry > 3.13f)
		ry = tiltAngle - 3.13f;
	else if (tiltAngle-ry < 0.01f)
		ry = tiltAngle - 0.01f;

	Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
	rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

	center = rotMat.Transform(center);
	direction = rotMat.TransformDirection(direction);

	// ===========================================
	// ASSIGNMENT 3: Fix any other affected values
	// ===========================================
	direction = direction.Normalize();
	up = up.Normalize();
	horizontal = Vec3f::Cross3(direction, up).Normalize();
}

// ====================================================================
// ====================================================================
