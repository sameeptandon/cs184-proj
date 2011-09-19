#include <Eigen/Core>
USING_PART_OF_NAMESPACE_EIGEN
using namespace std;

bool intersect(Vector3d& ray_orig, Vector3d& ray_dir, double& t, double sphere_radius)
{
    //Compute A, B and C coefficients
    double a = ray_dir.dot(ray_dir);
    double b = 2 * ray_dir.dot(ray_orig);
    double c = ray_orig.dot(ray_orig) - (sphere_radius * sphere_radius);

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
