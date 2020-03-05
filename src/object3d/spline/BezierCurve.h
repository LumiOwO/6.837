#ifndef _BEZIERCURVE_H_
#define _BEZIERCURVE_H_

#include "curve.h"

class BezierCurve : public Curve
{
public:
	static const Matrix B_Bezier;
	static const Matrix B_Bezier_inv;
public:
	BezierCurve(int n) :Curve(n) { }
	virtual ~BezierCurve() { }

	// FOR CONVERTING BETWEEN SPLINE TYPES
	virtual void OutputBezier(FILE *file) override;
	virtual void OutputBSpline(FILE *file) override;

	// FOR EDITING OPERATIONS
	virtual void addControlPoint(int selectedPoint, float x, float y) override {
		cout << "Refuse to add control points to Bezier Curve." << endl;
	}
	virtual void deleteControlPoint(int selectedPoint) override {
		cout << "Refuse to delete control points from Bezier Curve." << endl;
	}

protected:
	virtual int numOfUnits() const override {
		return (n - 1) / 3;
	}
	virtual vector<Vec3f> unitCP(int i) const override {
		int o = 3 * i;
		return { v[o], v[o+1], v[o+2], v[o+3] };
	}
	virtual const Matrix& B() const override {
		return B_Bezier;
	}
};

#endif
