#ifndef _TRIANGLE_H
#define _TRIANGLE_H
#include "Shape.h"

class Triangle : public Shape {
  protected:
    Vector3d _a, _b, _c;
  public:
    Triangle(Vector3d, Vector3d, Vector3d, Vector3d, double, Vector3d, Vector3d, Vector3d);
    ~Triangle();
    Vector3d normal(Vector3d& point);
    bool intersect(Vector3d& ray_orig, Vector3d& ray_dir, double &t);
}
#endif //_TRIANGLE_H
