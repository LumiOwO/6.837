#ifndef _BSPLINECURVE_H_
#define _BSPLINECURVE_H_

#include "curve.h"

class BSplineCurve : public Curve
{
public:
	static const Matrix B_BSpline;
	static const Matrix B_BSpline_inv;
public:
	BSplineCurve(int n) :Curve(n) { }
	virtual ~BSplineCurve() { }

	// FOR CONVERTING BETWEEN SPLINE TYPES
	virtual void OutputBezier(FILE *file) override;
	virtual void OutputBSpline(FILE *file) override;

protected:
	virtual int numOfUnits() const override {
		return n - 3;
	}
	virtual vector<Vec3f> unitCP(int i) const override {
		return { v[i], v[i + 1], v[i + 2], v[i + 3] };
	}
	virtual const Matrix& B() const override {
		return B_BSpline;
	}
};

#endif
