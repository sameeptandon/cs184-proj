#include "sphere.h"

sphere::sphere(Vector3d center, int radius) :
  _center(center),
  _radius(radius) {};

/**
 * This returns the vector from the center to the surface
 */
Vector3d sphere::normal(Vector3d point) {
  assert((point-_center).norm() - radius < 0.001);
  return point-_center;
}

/**
 * Adapted code from: http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection
 */
void sphere::intersect(Vector3d& ray_orig, Vector3d &ray_dir, double &t) {
  //Compute A, B and C coefficients
  double a = ray_dir.dot(ray_dir);
  double b = 2 * ray_dir.dot(ray_orig);
  double c = (ray_orig-_center).dot(ray_orig-_center) - (_radius * _radius);

  //Find discriminant
  double disc = b * b - 4 * a * c;

  // if discriminant is negative there are no real roots, so return 
  // false as ray misses sphere
  if (disc < 0)
    return false;

  // compute q as described above
  double distSqrt = sqrtf(disc);
  double q;
  if (b < 0)
    q = (-b - distSqrt)/2.0;
  else
    q = (-b + distSqrt)/2.0;

  // compute t0 and t1
  double t0 = q / a;
  double t1 = c / q;

  // make sure t0 is smaller than t1
  if (t0 > t1)
  {
    // if t0 is bigger than t1 swap them around
    double temp = t0;
    t0 = t1;
    t1 = temp;
  }

  // if t1 is less than zero, the object is in the ray's negative direction
  // and consequently the ray misses the sphere
  if (t1 < 0)
    return false;

  // if t0 is less than zero, the intersection point is at t1
  if (t0 < 0)
  {
    t = t1;
    return true;
  }
  // else the intersection point is at t0
  else
  {
    t = t0;
    return true;
  }

}

sphere::~sphere() {};
