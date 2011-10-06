#include "Camera.h"

Camera::Camera(Viewport &viewport, Ray &loc) :
  _viewport(viewport),
  _location(loc),
  _x(0),
  _y(0) {};

bool Camera::generateSample(Ray &r) {
  if( _x == _viewport.w - 1 && _y == _viewport.h-1 ) {
    _x = 0;
    _y = 0;
    return false;
  }
  // FIXME: Support rectangular screen and rotation matrix
  Vector3d v, origin;
  _location.getDirection(v);
  _location.getOrigin(origin);
  Vector3d dir = v + Vector3d(-1, -1, 0) + BOX_PARAM*Vector3d(((double) _x)/_viewport.w,((double) _y)/_viewport.h, 0);
  r = Ray(Vector2d(_x,_y), origin, dir, 0); 

  _x++;
  if( _x > _viewport.w - 1 ) {
    _x = 0;
    _y++;
  }
  return true;
}
