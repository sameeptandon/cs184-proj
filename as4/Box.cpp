#include "Box.h"

/*
 * Code adapted from: http://jgt.akpeters.com/papers/WilliamsEtAl05/
 */

bool Box::intersect(const Ray &r, double t0, double t1) const {
  double tmin, tmax, tymin, tymax, tzmin, tzmax;

  tmin = (parameters[r.sign[0]](0) - r._origin(0)) * r._inv_direction(0);
  tmax = (parameters[1-r.sign[0]](0) - r._origin(0)) * r._inv_direction(0);
  tymin = (parameters[r.sign[1]](1) - r._origin(1)) * r._inv_direction(1);
  tymax = (parameters[1-r.sign[1]](1) - r._origin(1)) * r._inv_direction(1);
  if ( (tmin > tymax) || (tymin > tmax) ) 
    return false;
  if (tymin > tmin)
    tmin = tymin;
  if (tymax < tmax)
    tmax = tymax;
  tzmin = (parameters[r.sign[2]](2) - r._origin(2)) * r._inv_direction(2);
  tzmax = (parameters[1-r.sign[2]](2) - r._origin(2)) * r._inv_direction(2);
  if ( (tmin > tzmax) || (tzmin > tmax) ) 
    return false;
  if (tzmin > tmin)
    tmin = tzmin;
  if (tzmax < tmax)
    tmax = tzmax;
  return ( (tmin < t1) && (tmax > t0) );
}
