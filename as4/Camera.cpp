#include "Camera.h"

Camera::Camera(Viewport &viewport, Vector3d &ll, Vector3d &lr, Vector3d &ul, Vector3d &loc, int aa_sampling) :
  _viewport(viewport),
  _ll(ll),
  _lr(lr),
  _ul(ul),
  _location(loc),
  _aa_sampling(aa_sampling),
  _aa_count(0),
  _x(0),
  _y(0) {};

bool Camera::generateSample(Ray &r) {
  if( _x == _viewport.w - 1 && _y == _viewport.h-1 ) {
    _x = 0;
    _y = 0;
    return false;
  }

  double x_perturb = drand48();
  double y_perturb = drand48();
  if (_aa_sampling == 1) {
    x_perturb = 0;
    y_perturb = 0;
  }

  Vector3d dir = -_location + _ll + (_lr-_ll)*((double) _x + x_perturb)/_viewport.w + (_ul-_ll)*((double) _y+ y_perturb)/_viewport.h;
  r = Ray(Vector2d(_x,_y), _location, dir, 0, Vector3d::Ones() / _aa_sampling); 

  _aa_count += 1;
  if(_aa_count == _aa_sampling) { 
    _aa_count = 0;
    _x++;
    if( _x > _viewport.w - 1 ) {
      _x = 0;
      _y++;
    }
  }
  return true;

}
