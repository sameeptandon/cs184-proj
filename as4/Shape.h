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
    inline double getKa() {return _ka;}
    inline double getKs() {return _ks;}
    inline double getKd() {return _kd;}
    inline double getKm() {return _km;}
};
#endif //_SHAPE_H
