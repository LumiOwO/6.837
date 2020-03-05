#ifndef _SPLINE_H_
#define _SPLINE_H_

#include "ce_arg_parser.h"
#include "triangle_mesh.h"

class Spline
{
public:
	virtual ~Spline() {}

	// FOR VISUALIZATION
	virtual void Paint(ArgParser *args) = 0;

	// FOR CONVERTING BETWEEN SPLINE TYPES
	virtual void OutputBezier(FILE *file) = 0;
	virtual void OutputBSpline(FILE *file) = 0;

	// FOR CONTROL POINT PICKING
	virtual int getNumVertices() = 0;
	virtual Vec3f getVertex(int i) = 0;

	// FOR EDITING OPERATIONS
	virtual void moveControlPoint(int selectedPoint, float x, float y) = 0;
	virtual void addControlPoint(int selectedPoint, float x, float y) = 0;
	virtual void deleteControlPoint(int selectedPoint) = 0;

	// FOR GENERATING TRIANGLES
	virtual TriangleMesh* OutputTriangles(ArgParser *args) = 0;
};

#endif
