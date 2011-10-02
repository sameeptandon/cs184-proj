#ifndef _DIRECTIONALLIGHT_H
#define _DIRECTIONALLIGHT_H
#include "Light.h"

class DirectionalLight {
  protected:
    Vector3d _direction;
  public:
    DirectionalLight(Vector3d, Vector3d);
    ~DirectionalLight();
    inline void getDirection(Vector3d& dir){dir = _direction;}
};

#endif //_DIRECTIONALLIGHT_H
