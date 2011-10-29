#include "Triangle.h"

Triangle::Triangle(Vector3d a, Vector3d b, Vector3d c) :
  _a(a),
  _b(b),
  _c(c) {
  
  //precompute normal 
  _normal = (_b-_a).cross(_c-_a);
  _normal = -_normal.normalized();
};

Triangle::~Triangle() {};

Vector3d Triangle::normal() {
  //we should assert that the point is on the triangle
  return _normal; 
}

void Triangle::Draw() {
  glBegin(GL_TRIANGLES);
  glNormal3d(_normal(0), _normal(1), _normal(2));
  glVertex3d(_a(0), _a(1), _a(2));
  glNormal3d(_normal(0), _normal(1), _normal(2));
  glVertex3d(_b(0), _b(1), _b(2));
  glNormal3d(_normal(0), _normal(1), _normal(2));
  glVertex3d(_c(0), _c(1), _c(2));
  glEnd();
}
