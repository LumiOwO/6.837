#ifndef _SURFACEOFREVOLUTION_H_
#define _SURFACEOFREVOLUTION_H_

#include "surface.h"
#include "curve.h"

class SurfaceOfRevolution: public Surface
{
private:
	Curve *c;
public:
	SurfaceOfRevolution(Curve *c): c(c) {}
	virtual ~SurfaceOfRevolution() { delete c; }

	// FOR VISUALIZATION
	virtual void Paint(ArgParser *args) override { c->Paint(args); }
	// FOR CONTROL POINT PICKING
	virtual int getNumVertices() override { return c->getNumVertices(); }
	virtual Vec3f getVertex(int i) override { return c->getVertex(i); }

	// FOR EDITING OPERATIONS
	virtual void moveControlPoint(int selectedPoint, float x, float y) override {
		c->moveControlPoint(selectedPoint, x, y);
	}
	virtual void addControlPoint(int selectedPoint, float x, float y) override {
		c->addControlPoint(selectedPoint, x, y);
	}
	virtual void deleteControlPoint(int selectedPoint) override {
		c->deleteControlPoint(selectedPoint);
	}
	// FOR CONVERTING BETWEEN SPLINE TYPES
	virtual void OutputBezier(FILE *file) override {
		fprintf(file, "surface_of_revolution\n");
		c->OutputBezier(file);
	}
	virtual void OutputBSpline(FILE *file) override { 
		fprintf(file, "surface_of_revolution\n");
		c->OutputBSpline(file);
	}
	// FOR GENERATING TRIANGLES
	virtual TriangleMesh* OutputTriangles(ArgParser *args) override;
};

#endif
