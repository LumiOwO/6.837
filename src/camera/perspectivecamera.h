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

	virtual Ray generateRay(Vec2f point) const override;
	virtual float getTMin() const override { return 0; }

	virtual void glInit(int w, int h) const override;
	virtual void glPlaceCamera(void) const override;
	virtual void dollyCamera(float dist) override;
	virtual void truckCamera(float dx, float dy) override;
	virtual void rotateCamera(float rx, float ry) override;
};

#endif // PERSPECTIVECAMERA_H
