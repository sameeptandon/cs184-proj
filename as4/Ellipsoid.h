#ifndef _ELLIPSOID_H
#define _ELLIPSOID_H
#include "Sphere.h"

class Ellipsoid : public Sphere {
  protected:
    // Scale: x,y,z radii
    // Translate: x,y,z from origin
    // Rotation: about x,y,z axes
    Vector3d _scale, _translation, _rotation;
    // M is R * S
    Matrix3d _M, _rot;
    Matrix3d _M_inverse;
    Sphere _sphere;
  public:
    Ellipsoid(Vector3d, Vector3d, Vector3d, Vector3d, double, Vector3d, Vector3d, Vector3d);
    ~Ellipsoid();
    Vector3d normal(Vector3d);
    bool intersect(Ray&, double&);
};
#endif //_ELLIPSOID_H
