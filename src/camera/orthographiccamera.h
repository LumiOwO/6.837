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

	virtual Ray generateRay(Vec2f point);
	virtual float getTMin() const { return -FLT_MAX; }

	virtual void glInit(int w, int h);
	virtual void glPlaceCamera(void);
	virtual void dollyCamera(float dist);
	virtual void truckCamera(float dx, float dy);
	virtual void rotateCamera(float rx, float ry);
};

#endif // ORTHOGRAPHICCAMERA_H
