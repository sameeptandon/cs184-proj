#include "Shape.h"
#include <cfloat>

bool Shape::bbIntersect(Ray& r) {
  return _bb.intersect(r, 0, DBL_MAX);
}
