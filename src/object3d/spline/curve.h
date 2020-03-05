#ifndef _CURVE_H_
#define _CURVE_H_

#include "spline.h"
#include "matrix.h"
#include <vector>

class Curve: public Spline
{
protected:
	int n;
	vector<Vec3f> v;
public:
	Curve(int n) :n(n), v(n) { assert(n >= 4); }
	virtual ~Curve() { }

	void set(int i, const Vec3f &p) { v[i] = p; }

	// FOR VISUALIZATION
	virtual void Paint(ArgParser *args) override;

	// FOR CONTROL POINT PICKING
	virtual int getNumVertices() override { return n; }
	virtual Vec3f getVertex(int i) override { return v[i]; }

	// FOR EDITING OPERATIONS
	virtual void moveControlPoint(int selectedPoint, float x, float y) override {
		v[selectedPoint] = { x, y, 0 };
	}
	virtual void addControlPoint(int selectedPoint, float x, float y) override {
		v.insert(v.begin() + selectedPoint, { x, y, 0 });
		n++;
	}
	virtual void deleteControlPoint(int selectedPoint) override {
		if (v.size() == 4) {
			cout << "Refuse to delete because " <<
				"the number of control points must be no less than 4." << endl;
			return;
		}
		v.erase(v.begin() + selectedPoint);
		n--;
	}

	// FOR GENERATING TRIANGLES
	virtual TriangleMesh* OutputTriangles(ArgParser *args) override { 
		return new TriangleMesh(0, 0); 
	}
	Vec3f Q(float t, int unit) const;
	virtual int numOfUnits() const = 0;

protected:
	virtual vector<Vec3f> unitCP(int i) const = 0;
	virtual const Matrix& B() const = 0;

	void OutputCP(FILE *f, const vector<Vec3f>& cp) const {
		fprintf(f, "num_vertices %d\n", cp.size());
		for (Vec3f p : cp) {
			fprintf(f, "%f %f %f\n", p.x(), p.y(), p.z());
		}
	}
};

#endif
