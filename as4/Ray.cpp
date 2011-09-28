#include "Ray.h"

Ray::Ray(Vector2d pixel, Vector3d origin, Vector3d direction, int depth) :
  _pixel(pixel) ,
  _origin(origin) ,
  _direction(direction),
  _depth(depth) {};

Ray::~Ray() {};
