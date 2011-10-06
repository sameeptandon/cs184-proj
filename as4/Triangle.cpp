#include "Triangle.h"
#define INTERSECTION_EPS 0.0000001


Triangle::Triangle(Vector3d ka, Vector3d kd, Vector3d ks, Vector3d km, double sp, Vector3d a, Vector3d b, Vector3d c) :
  _a(a),
  _b(b),
  _c(c) {
  
  _ka = ka;
  _kd = kd;
  _ks = ks;
  _km = km;
  _sp = sp;

  //precompute normal 
  _normal = (_c-_a).cross(_c-_b);
  if(_normal[2] > 0.0) // We want normal to face the camera (at origin)
    _normal = -_normal;
  
  };

Triangle::~Triangle() {};

Vector3d Triangle::normal(Vector3d point) {
  //we should assert that the point is on the triangle
  return _normal; 
}

//Follows: http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
bool Triangle::intersect(Ray& r, double &t) {
  Vector3d ray_origin, ray_direction;
  r.getOrigin(ray_origin);
  r.getDirection(ray_direction);

  //(vert0, vert1, vert2) = (_a, _b_, c)

  bool intersect = true; 
  Vector3d edge1 = _b-_a;
  Vector3d edge2 = _c-_a;
  Vector3d pvec = ray_direction.cross(edge2);
  double det = pvec.dot(edge1);

  if (det > -INTERSECTION_EPS && det < INTERSECTION_EPS) 
    return false; 

  double inv_det = 1.0 / det; 
  Vector3d tvec = ray_origin - _a; 
  double u = tvec.dot(pvec) * inv_det;
  if ( u < 0.0 || u > 1.0) return false;
  Vector3d qvec = tvec.cross(edge1);
  double v = ray_direction.dot(qvec) * inv_det;
  if ( v < 0.0 || (u+v) > 1.0) return false;  
  t = edge2.dot(qvec) * inv_det;

  return intersect;

}
