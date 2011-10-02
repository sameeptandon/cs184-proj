#ifndef _POINTLIGHT_H
#define _POINTLIGHT_H
#include "Light.h"

class PointLight {
  protected:
    Vector3d _position;
  public:
    PointLight(Vector3d, Vector3d);
    ~PointLight();
    inline void getPosition(Vector3d& pos){pos = _position;}
};

#endif //_POINTLIGHT_H
