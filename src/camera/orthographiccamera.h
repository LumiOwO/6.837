#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include "camera.h"

class OrthographicCamera: public Camera
{
private:
	Vec3f center;
	Vec3f direction;
	Vec3f up;
	Vec3f horizontal;
	float size;
public:
	OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size);
	virtual ~OrthographicCamera() {}

	virtual Ray generateRay(Vec2f point) const override;
	virtual float getTMin() const override { return -FLT_MAX; }

	virtual void glInit(int w, int h) const override;
	virtual void glPlaceCamera(void) const override;
	virtual void dollyCamera(float dist) override;
	virtual void truckCamera(float dx, float dy) override;
	virtual void rotateCamera(float rx, float ry) override;
};

#endif // ORTHOGRAPHICCAMERA_H
