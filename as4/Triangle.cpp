#include "Triangle.h"

Triangle::Triangle(Vector3d ka, Vector3d kd, Vector3d ks, Vector3d km, double sp, Vector3d a, Vector3d b, Vector3d) :
  _ka(ka),
  _kd(kd),
  _ks(ks),
  _km(km),
  _sp(sp),
  _a(a),
  _b(b),
  _c(c) {};

Triangle::~Triangle() {};

Vector3d Triangle::normal(Vector3d& point) {
  Vector3d cross = (_c-_a).cross(_c-_b);
  if(cross[2] < 0.0) // We want normal to face the camera (at origin)
    return -cross;
  else return cross;
}

Triangle::intersect(Vector3d& ray_orig, Vector3d& ray_dir, double &t) {
  return false;
}
