#include "triangle.h"

triangle::triangle(Vector3d a, Vector3d b, Vector3d) :
  _a(a),
  _b(b),
  _c(c) {};

triangle::~triangle() {};

Vector3d triangle::normal(Vector3d point) {
  return NULL;
}

triangle::intersect(Vector3d& ray_orig, Vector3d& ray_dir, double &t) {
  return false;
}
