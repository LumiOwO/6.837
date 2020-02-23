#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "camera.h"

class PerspectiveCamera: public Camera
{
private:
	Vec3f center;
	Vec3f direction;
	Vec3f up;
	Vec3f horizontal;
	float angle_radians;
public:
	PerspectiveCamera(Vec3f center, Vec3f direction, Vec3f up, float angle_radians);
	virtual ~PerspectiveCamera() {}

	virtual Ray generateRay(Vec2f point);
	virtual float getTMin() const { return 0; }

	virtual void glInit(int w, int h);
	virtual void glPlaceCamera(void);
	virtual void dollyCamera(float dist);
	virtual void truckCamera(float dx, float dy);
	virtual void rotateCamera(float rx, float ry);
};

#endif // PERSPECTIVECAMERA_H
