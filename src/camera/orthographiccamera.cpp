#include "orthographiccamera.h"
#include "matrix.h"
#include <GL/glut.h>

OrthographicCamera::OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size):
	center(center), direction(direction.Normalize()),
	up(up.Normalize()), size(size)
{
	horizontal = Vec3f::Cross3(direction, up).Normalize();
}

Ray OrthographicCamera::generateRay(Vec2f point) const
{
	float x, y;
	point.Get(x, y);
//	cout << x << ", " << y << endl;
	Vec3f origin = center +
				   (x - 0.5f) * size * horizontal +
				   (y - 0.5f) * size * up;

	return Ray(origin, direction);
}

// ====================================================================
// Create an orthographic camera with the appropriate dimensions that
// crops the screen in the narrowest dimension.
// ====================================================================

void OrthographicCamera::glInit(int w, int h) const
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w > h)
		glOrtho(
			-(double)size/2.0, (double)size/2.0,
			-(double)size*h/w/2.0, (double)size*h/w/2.0,
			0.5, 40.0
		);
	else
		glOrtho(
			-(double)size*w/h/2.0, (double)size*w/h/2.0,
			-(double)size/2.0, (double)size/2.0,
			0.5, 40.0
		);
}

// ====================================================================
// Place an orthographic camera within an OpenGL scene
// ====================================================================

void OrthographicCamera::glPlaceCamera(void) const
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
//    values at athe end of the these three routines
// ====================================================================

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void OrthographicCamera::dollyCamera(float dist)
{
	center += direction*dist;

	// ===========================================
	// ASSIGNMENT 3: Fix any other affected values
	// ===========================================


}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void OrthographicCamera::truckCamera(float dx, float dy)
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

void OrthographicCamera::rotateCamera(float rx, float ry)
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
