#include "Ellipsoid.h"

/*void rotation_from_euler_angles(Matrix3d& rotation, double angZ, double angY, double angX)
{
  Matrix3d rot1(Eigen::AngleAxisd(angZ, Vector3d::UnitZ()));
  Vector3d axis2 = rot1*(Vector3d::UnitY());
  Matrix3d rot2 = Eigen::AngleAxisd(angY, axis2)*rot1;
  Vector3d axis3 = rot2*(Vector3d::UnitX());
  rotation = Eigen::AngleAxisd(angX, axis3)*rot2;
}*/

void rotation_from_euler_angles(Matrix3d& rotation, double angZ, double angY, double angX)
{
  Matrix3d rot1(Eigen::AngleAxisd(angX, Vector3d::UnitX()));
  Vector3d axis2 = rot1*(Vector3d::UnitY());
  Matrix3d rot2 = Eigen::AngleAxisd(angY, axis2)*rot1;
  Vector3d axis3 = rot2*(Vector3d::UnitZ());
  rotation = Eigen::AngleAxisd(angZ, axis3)*rot2;
}
void euler_angles_from_rotation(const Matrix3d& rotation, double& angZ, double& angY, double& angX)
{
 /* angZ = atan2(rotation(1,0), rotation(0,0));
  angY = atan2(-rotation(2,0), sqrt(rotation(2,1)*rotation(2,1) + rotation(2,2)*rotation(2,2)));
  angX = atan2(rotation(2,1), rotation(2,2));
  */
}

Ellipsoid::Ellipsoid(Vector3d ka, Vector3d kd, Vector3d ks, Vector3d km, double sp, Vector3d scale, Vector3d translation, Vector3d rotation) :
  _scale(scale),
  _translation(translation),
  _rotation(rotation)
{
  _center = Vector3d(0.0, 0.0, 0.0);
  _radius = 1.0;
  _ka = ka;
  _kd = kd;
  _ks = ks;
  _km = km;
  _sp = sp;
  // Calculate _M and _M_inverse
  rotation_from_euler_angles(_M_rot, rotation(0), rotation(1), rotation(2));
  _M_scale = Matrix3d::Zero();
  for (int i = 0; i < 3; i++) { 
    _M_scale(i,i) = _scale(i);
  }
  _M = _M_rot * _M_scale;

  _M_scale_inv = _M_scale.inverse();
  _M_rot_inv = _M_rot.inverse();
  _M_inv = _M.inverse();
  // Calculate bounding box
  _bb = Box( translation - scale, translation + scale );
};

/**
 * This returns the vector from the center to the surface
 */
Vector3d Ellipsoid::normal(Vector3d point) {
  //std::cout << (point-_center).norm() - _radius << std::endl;
  //assert((point-_translation).norm() - _scale.norm() < 0.001);
  Vector3d unit_normal = _M_inv * (point-_translation);
  assert(unit_normal.norm() - 1.0 < 0.001);
  return _M_rot*_M_scale_inv*unit_normal;
}

bool Ellipsoid::intersect(Ray& r, double &t) {
  if( bbIntersect(r) ) {
    Vector3d ray_dir, ray_orig;
    Vector3d ray_dir_unit_sphere, ray_orig_unit_sphere;
    r.getDirection(ray_dir);
    r.getOrigin(ray_orig);
    ray_dir_unit_sphere = _M_inv * (ray_dir);
    ray_orig_unit_sphere = _M_inv * (ray_orig - _translation);
    //Ray r_unit_sphere = Ray(Vector2d(0,0), ray_dir_unit_sphere, ray_orig_unit_sphere, 0);
    /*
     * cout << ray_dir.transpose() << endl;
     cout << ray_dir_unit_sphere.transpose() << endl;
     cout << ray_orig.transpose() << endl;
     cout << ray_orig_unit_sphere.transpose() << endl;
     exit(0);
     */
    //Compute A, B and C coefficients
    double a = ray_dir_unit_sphere.dot(ray_dir_unit_sphere);
    double b = 2 * ray_dir_unit_sphere.dot(ray_orig_unit_sphere-_center);
    double c = (ray_orig_unit_sphere-_center).dot(ray_orig_unit_sphere-_center) - (_radius * _radius);

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


