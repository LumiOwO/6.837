#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include <cfloat>

class Camera
{
public:
	Camera() {}
	virtual ~Camera() {}

	virtual Ray generateRay(Vec2f point) const = 0;
	virtual float getTMin() const = 0;

	virtual void glInit(int w, int h) const = 0;
	virtual void glPlaceCamera(void) const = 0;
	virtual void dollyCamera(float dist) = 0;
	virtual void truckCamera(float dx, float dy) = 0;
	virtual void rotateCamera(float rx, float ry) = 0;
};
#endif // CAMERA_H
