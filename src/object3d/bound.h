#ifndef _BOUND_H_
#define _BOUND_H_

#include "vectors.h"

#include <assert.h>

class Ray;
class Hit;

// ====================================================================
// ====================================================================

class Bound {

public:

  // CONSTRUCTOR & DESTRUCTOR
  Bound(Vec3f p): valid(true) { Set(p, p); }
  Bound(Vec3f p1, Vec3f p2): valid(true),
	  min(min2(p1.x(), p2.x()), min2(p1.y(), p2.y()), min2(p1.z(), p2.z())),
	  max(max2(p1.x(), p2.x()), max2(p1.y(), p2.y()), max2(p1.z(), p2.z())) { }
  Bound(const Bound &b);
  ~Bound() {}

  // ACCESSORS
  void Get(Vec3f &_min, Vec3f &_max) const {
    _min = min;
    _max = max; }
  Vec3f getMin() const { return min; }
  Vec3f getMax() const { return max; }
  Vec3f diagonal() const { return max - min; }
  Vec3f diagonal(int nx, int ny, int nz) const { 
	  Vec3f d = diagonal();
	  d.Divide(nx, ny, nz);
	  return d; 
  }
  Vec3f center() const { return (max + min) / 2; }
  bool isValid() const { return valid; }
  static Bound invalid() { return Bound(false); }

  // MODIFIERS
  void Set(Bound *bb) {
    assert(bb != NULL);
    min = bb->min;
    max = bb->max; }
  void Set(Vec3f _min, Vec3f _max) {
    assert (min.x() <= max.x() &&
	    min.y() <= max.y() &&
	    min.z() <= max.z());
    min = _min;
    max = _max; }
  Bound Extend(const Vec3f &v) const {
    Vec3f new_min = Vec3f(min2(min.x(),v.x()),
		min2(min.y(),v.y()),
		min2(min.z(),v.z()));
	Vec3f new_max = Vec3f(max2(max.x(),v.x()),
		max2(max.y(),v.y()),
		max2(max.z(),v.z()));
	return Bound(new_min, new_max);
  }
  Bound Extend(const Bound &bb) const {
    return this->Extend(bb.min).Extend(bb.max);
  }

  // utilities
  bool cover(Vec3f point) const;
  bool intersect(const Ray &r, Hit &h, float tmin) const;
  bool intersect(const Bound& box) const;
  Bound& operator=(const Bound& m);
  friend ostream& operator <<(ostream& out, const Bound& box) {
	  out << "min: " << box.min << ", max: " << box.max;
	  return out;
  }

  // DEBUGGING 
  void Print() const {
    printf ("%f %f %f  -> %f %f %f\n", min.x(),min.y(),min.z(),
	    max.x(),max.y(),max.z()); }
  void paint() const;

private:

  Bound() { assert(0); } // don't use this constructor
  Bound(bool b) { valid = false; }

  // REPRESENTATION
  Vec3f min;
  Vec3f max;
  bool valid;

};

// ====================================================================
// ====================================================================

#endif
