#ifndef _TRIANGLE_H
#define _TRIANGLE_H
#include "Shape.h"

class Triangle : public Shape {
  protected:
    Vector3d _a, _b, _c, _normal;
  public:
    Triangle(Vector3d, Vector3d, Vector3d, Vector3d, double, Vector3d, Vector3d, Vector3d);
    ~Triangle();
    Vector3d normal(Vector3d point);
    bool intersect(Ray&, double &);
};
#endif //_TRIANGLE_H
