#ifndef _SHAPE_H
#define _SHAPE_H
/*
 *  Description:  Basic shape abstract class, defines the methods that shapes will have
*/
#include "Ray.h"
#include "Box.h"

class Shape {
  protected:
    Vector3d _ka, _ks, _kd, _km;
    double _sp;
    Box _bb;
    bool bbIntersect(Ray&);
  public:
    inline void getKa(Vector3d& ka) {ka = _ka;}
    inline void getKs(Vector3d& ks) {ks = _ks;}
    inline void getKd(Vector3d& kd) {kd = _kd;}
    inline void getKm(Vector3d& km) {km = _km;}
    inline void getSp(double& sp) {sp = _sp;}
    virtual Vector3d normal(Vector3d point) = 0;
    virtual bool intersect(Ray&, double&) = 0;
};
#endif //_SHAPE_H
