#include "Ray.h"

Ray::Ray(Vector2d pixel, Vector3d origin, Vector3d direction, int depth, Vector3d scale) :
  _pixel(pixel) ,
  _origin(origin) ,
  _direction(direction),
  _depth(depth),
  _scale(scale) {};

Ray::Ray(Vector2d pixel, Vector3d origin, Vector3d direction, int depth) :
  _pixel(pixel) ,
  _origin(origin) ,
  _direction(direction),
  _depth(depth)
{
  _scale = Vector3d(1.0, 1.0, 1.0);
};

Ray::~Ray() {};
