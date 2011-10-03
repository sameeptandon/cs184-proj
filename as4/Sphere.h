#ifndef _SPHERE_H
#define _SPHERE_H
#include "Shape.h"

class Sphere : public Shape {
  protected:
    Vector3d _center;
    int _radius;
  public:
    Sphere(Vector3d, Vector3d, Vector3d, Vector3d, double, Vector3d, int);
    ~Sphere();
    Vector3d normal(Vector3d);
    bool intersect(Ray&, double&);
};
#endif //_SPHERE_H
