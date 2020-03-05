#ifndef _MATRIX_H_
#define _MATRIX_H_

//
// originally implemented by Justin Legakis
//

#include <vector>
#include <math.h>
#include <assert.h>

#include "vectors.h"
#include "bound.h"
#include "ray.h"

// ====================================================================
// ====================================================================

class Matrix {

public:

	static float det4x4(float a1, float a2, float a3, float a4,
			 float b1, float b2, float b3, float b4,
			 float c1, float c2, float c3, float c4,
			 float d1, float d2, float d3, float d4);
	static float det3x3(float a1,float a2,float a3,
			 float b1,float b2,float b3,
			 float c1,float c2,float c3);
	static float det2x2(float a, float b,
			 float c, float d);

  // CONSTRUCTORS & DESTRUCTOR
  Matrix() { SetToIdentity(); }
  Matrix(const Matrix& m);
  Matrix(const float *m);
  Matrix(initializer_list<float> list);
  ~Matrix() {}

  static Matrix Identity() { return Matrix(); }
  static Matrix Zeros() { Matrix m; m.Clear(); return m; }
  
  // ACCESSORS
  float* glGet(void) const {
    float *glMat = new float[16];
     glMat[0]=data[0][0];  glMat[1]=data[1][0];  glMat[2]=data[2][0];  glMat[3]=data[3][0];
     glMat[4]=data[0][1];  glMat[5]=data[1][1];  glMat[6]=data[2][1];  glMat[7]=data[3][1];
     glMat[8]=data[0][2];  glMat[9]=data[1][2]; glMat[10]=data[2][2]; glMat[11]=data[3][2];
    glMat[12]=data[0][3]; glMat[13]=data[1][3]; glMat[14]=data[2][3]; glMat[15]=data[3][3];
    return glMat; }
  float Get(int x, int y) const { 
    assert (x >= 0 && x < 4);
    assert (y >= 0 && y < 4);
    return data[y][x]; }
  
  // MODIFIERS
  void Set(int x, int y, float v) {
    assert (x >= 0 && x < 4);
    assert (y >= 0 && y < 4);
    data[y][x] = v; }
  void SetToIdentity();
  void Clear();

  Matrix Transpose() const;
  Matrix Inverse(float epsilon = 1e-08f) const;

  // OVERLOADED OPERATORS
  Matrix& operator=(const Matrix& m);
  int operator==(const Matrix& m) const;
  int operator!=(const Matrix &m) const { return !(*this==m); }
  friend Matrix operator+(const Matrix &m1, const Matrix &m2);
  friend Matrix operator-(const Matrix &m1, const Matrix &m2);
  friend Matrix operator*(const Matrix &m1, const Matrix &m2);
  friend Matrix operator*(const Matrix &m1, float f);
  friend Matrix operator*(float f, const Matrix &m) { return m * f; }
  Matrix& operator+=(const Matrix& m) { *this = *this + m; return *this; }
  Matrix& operator-=(const Matrix& m) { *this = *this - m; return *this; }
  Matrix& operator*=(const float f)   { *this = *this * f; return *this; }
  Matrix& operator*=(const Matrix& m) { *this = *this * m; return *this; }

  // TRANSFORMATIONS
  static Matrix MakeTranslation(const Vec3f &v);
  static Matrix MakeScale(const Vec3f &v);
  static Matrix MakeScale(float s) { return MakeScale(Vec3f(s,s,s)); }
  static Matrix MakeXRotation(float theta);
  static Matrix MakeYRotation(float theta);
  static Matrix MakeZRotation(float theta);
  static Matrix MakeAxisRotation(const Vec3f &v, float theta);
  static Matrix MakePermute(int x, int y, int z);

  // Use to transform a point with a matrix
  // that may include translation
  Vec4f Transform(const Vec4f &v) const;
  Vec3f Transform(const Vec3f &v) const {
    Vec4f v2 = Vec4f(v.x(),v.y(),v.z(),1);
    v2 = Transform(v2);
	return Vec3f(v2.x(), v2.y(), v2.z());}
  Vec2f Transform(const Vec2f &v) const {
    Vec4f v2 = Vec4f(v.x(),v.y(),1,1);
    v2 = Transform(v2);
	return Vec2f(v2.x(),v2.y()); }

  Vec4f operator()(const Vec4f &v) const { return Transform(v); }
  Vec3f operator()(const Vec3f &v) const { return Transform(v); }
  Vec2f operator()(const Vec2f &v) const { return Transform(v); }
  Bound operator()(const Bound &b) const;
  Ray operator()(const Ray &r) const;
  Vec4f operator*(const Vec4f &v) const { return Transform(v); }
  
  // type convert
  vector<Vec4f> toVec4fs() const {
	  vector<Vec4f> ret;
	  for (int i = 0; i < 4; i++) {
		  ret.push_back(Vec4f(
			  data[0][i], data[1][i], data[2][i], data[3][i]
		  ));
	  }
	  return ret;
  }
  vector<Vec3f> toVec3fs() const {
	  vector<Vec3f> ret;
	  for (int i = 0; i < 4; i++) {
		  ret.push_back(Vec3f(
			  data[0][i], data[1][i], data[2][i]
		  ));
	  }
	  return ret;
  }

  // Use to transform the direction of the ray
  // (ignores any translation)
  Vec3f TransformDirection(const Vec3f &v) const {
    Vec4f v2 = Vec4f(v.x(),v.y(),v.z(),0);
	v2 = Transform(v2);
	return Vec3f(v2.x(),v2.y(),v2.z()); }
  // use to transform normal
  Vec3f TransformNormal(const Vec3f &n, const Matrix *inv = nullptr) const;

  // INPUT / OUTPUT
  void Write(FILE *F = stdout) const;
  void Write3x3(FILE *F = stdout) const;
  void Read(FILE *F);
  void Read3x3(FILE *F);

  friend inline ostream &operator<<(ostream &os, const Matrix &m) {
	  os << "Matrix: [" << endl;
	  for (int i = 0; i < 4; i++) {
		  for (int j = 0; j < 4; j++) {
			  os << "\t" << m.data[i][j];
		  }
		  os << endl;
	  }
	  os << "]" << endl;
	  return os;
  }
  
private:

  // REPRESENTATION
  float	data[4][4];

};


// ====================================================================
// ====================================================================

#endif
