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
    Matrix3d _M, _M_scale, _M_rot;
    Matrix3d _M_inv, _M_scale_inv, _M_rot_inv;
    Sphere _sphere;
  public:
    Ellipsoid(Vector3d, Vector3d, Vector3d, Vector3d, Vector3d, double, double, Vector3d, Vector3d, Vector3d);
    ~Ellipsoid();
    Vector3d normal(Vector3d);
    bool intersect(Ray&, double&);
};
#endif //_ELLIPSOID_H
