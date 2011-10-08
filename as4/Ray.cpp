#include "Ray.h"

Ray::Ray(Vector2d pixel, Vector3d origin, Vector3d direction, int depth, Vector3d scale, Shape* ignore_shape) :
  _pixel(pixel) ,
  _origin(origin) ,
  _direction(direction),
  _depth(depth),
  _scale(scale),
  _ignore_shape(ignore_shape) 
{
  _inv_direction = Vector3d(1.0/direction(0), 1.0/direction(1), 1.0/direction(2));
  sign[0] = (_inv_direction(0) < 0);
  sign[1] = (_inv_direction(1) < 0);
  sign[2] = (_inv_direction(2) < 0);
};

Ray::Ray(Vector2d pixel, Vector3d origin, Vector3d direction, int depth, Vector3d scale) :
  _pixel(pixel) ,
  _origin(origin) ,
  _direction(direction),
  _depth(depth),
  _scale(scale) 
{
  _ignore_shape = NULL;
  _inv_direction = Vector3d(1.0/direction(0), 1.0/direction(1), 1.0/direction(2));
  sign[0] = (_inv_direction(0) < 0);
  sign[1] = (_inv_direction(1) < 0);
  sign[2] = (_inv_direction(2) < 0);
};

Ray::Ray(Vector2d pixel, Vector3d origin, Vector3d direction, int depth) :
  _pixel(pixel) ,
  _origin(origin) ,
  _direction(direction),
  _depth(depth)
{
  _scale = Vector3d(1.0, 1.0, 1.0);
  _ignore_shape = NULL;
  _inv_direction = Vector3d(1.0/direction(0), 1.0/direction(1), 1.0/direction(2));
  sign[0] = (_inv_direction(0) < 0);
  sign[1] = (_inv_direction(1) < 0);
  sign[2] = (_inv_direction(2) < 0);
};

Ray::Ray(Vector2d pixel, Vector3d origin, Vector3d direction, int depth, Shape* ignore_shape) :
  _pixel(pixel) ,
  _origin(origin) ,
  _direction(direction),
  _depth(depth),
  _ignore_shape(ignore_shape) 
{

  _scale = Vector3d(1.0, 1.0, 1.0);
  _inv_direction = Vector3d(1.0/direction(0), 1.0/direction(1), 1.0/direction(2));
  sign[0] = (_inv_direction(0) < 0);
  sign[1] = (_inv_direction(1) < 0);
  sign[2] = (_inv_direction(2) < 0);
 

};

Ray::~Ray() {};
