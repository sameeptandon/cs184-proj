#ifndef _SHAPE_H
#define _SHAPE_H
/*
 *  Description:  Basic shape abstract class, defines the methods that shapes will have
*/
#include "Ray.h"

class Shape {
  protected:
    double _ka, _ks, _kd, _km;
  public:
    inline Vector3d normal(Vector3d point) {assert(false); return Vector3d();}
    virtual bool intersect(Ray&, double&) {assert(false); return false;}
    inline void getKa(Vector3d& ka) {ka = _ka;}
    inline void getKs(Vector3d& ks) {ks = _ks;}
    inline void getKd(Vector3d& kd) {kd = _kd;}
    inline void getKm(Vector3d& km) {km = _km;}
};
#endif //_SHAPE_H
