#include "Ray.h"

Ray::Ray(Vector2d pixel, Vector3d origin, Vector3d direction, int depth, Vector3d scale, Shape* ignore_shape) :
  _pixel(pixel) ,
  _origin(origin) ,
  _direction(direction),
  _depth(depth),
  _scale(scale),
  _ignore_shape(ignore_shape) {};

Ray::Ray(Vector2d pixel, Vector3d origin, Vector3d direction, int depth, Vector3d scale) :
  _pixel(pixel) ,
  _origin(origin) ,
  _direction(direction),
  _depth(depth),
  _scale(scale) 
{
  _ignore_shape = NULL;
};

Ray::Ray(Vector2d pixel, Vector3d origin, Vector3d direction, int depth) :
  _pixel(pixel) ,
  _origin(origin) ,
  _direction(direction),
  _depth(depth)
{
  _scale = Vector3d(1.0, 1.0, 1.0);
  _ignore_shape = NULL;
};

Ray::Ray(Vector2d pixel, Vector3d origin, Vector3d direction, int depth, Shape* ignore_shape) :
  _pixel(pixel) ,
  _origin(origin) ,
  _direction(direction),
  _depth(depth),
  _ignore_shape(ignore_shape) 
{

  _scale = Vector3d(1.0, 1.0, 1.0);

};
Ray::~Ray() {};
