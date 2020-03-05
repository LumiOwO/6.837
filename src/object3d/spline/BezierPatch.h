#ifndef _BEZIERPATCH_H_
#define _BEZIERPATCH_H_

#include "surface.h"

class BezierPatch: public Surface
{
private:
	const int n;
	Vec3f v[16];
public:
	BezierPatch() :n(16) { }
	virtual ~BezierPatch() { }

	void set(int i, const Vec3f &p) { v[i] = p; }

	// FOR VISUALIZATION
	virtual void Paint(ArgParser *args) override;
	// FOR CONTROL POINT PICKING
	virtual int getNumVertices() override { return 0; }
	virtual Vec3f getVertex(int i) override { return Vec3f(0, 0, 0); }
	// FOR EDITING OPERATIONS
	virtual void moveControlPoint(int selectedPoint, float x, float y) override { }
	virtual void addControlPoint(int selectedPoint, float x, float y) override { }
	virtual void deleteControlPoint(int selectedPoint) override { }
	// FOR CONVERTING BETWEEN SPLINE TYPES
	virtual void OutputBezier(FILE *file) override {}
	virtual void OutputBSpline(FILE *file) override {}
	// FOR GENERATING TRIANGLES
	virtual TriangleMesh* OutputTriangles(ArgParser *args) override;
private:
	Vec3f Q(float t, const Vec3f *cp);
};

#endif
