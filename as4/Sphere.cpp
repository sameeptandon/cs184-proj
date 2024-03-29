#include "Sphere.h"

Sphere::Sphere(Vector3d ka, Vector3d kd, Vector3d ks, Vector3d km, Vector3d kf, double rf_ind, double sp, Vector3d center, double radius) :
  _center(center),
  _radius(radius) 
{
  _ka = ka;
  _kd = kd;
  _ks = ks;
  _km = km;
  _kf = kf;
  _rf_ind = rf_ind;
  _sp = sp;
  _bb = Box( center - Vector3d::Ones()*radius, center + Vector3d::Ones()*radius );
};

/**
 * This returns the vector from the center to the surface
 */
Vector3d Sphere::normal(Vector3d point) {
  //std::cout << (point-_center).norm() - _radius << std::endl;
  assert((point-_center).norm() - _radius < 0.001);
  return point-_center;
}

/**
 * Adapted code from: http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection
 */
bool Sphere::intersect(Ray& r, double &t) {
  if( IGNOREBB || bbIntersect(r) ) {
    Vector3d ray_dir, ray_orig;
    r.getDirection(ray_dir);
    r.getOrigin(ray_orig);

    //Compute A, B and C coefficients
    double a = ray_dir.dot(ray_dir);
    double b = 2 * ray_dir.dot(ray_orig-_center);
    double c = (ray_orig-_center).dot(ray_orig-_center) - (_radius * _radius);

    //Find discriminant
    double disc = b * b - 4 * a * c;

    // if discriminant is negative there are no real roots, so return 
    // false as ray misses sphere
    if (disc < 0)
      return false;

    // compute q as described above
    double distSqrt = sqrt(disc);
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
  else {
    return false;
  }
}

Sphere::~Sphere() {};
