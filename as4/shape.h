/*
 *  Description:  Basic shape abstract class, defines the methods that shapes will have
*/

#include <Eigen/Core>
#include <Eigen/Geometry>

class shape {
  protected:
    double ka, ks, kd, km;
  public:
    virtual Vector3d normal(Vector3d point) = 0;
    virtual bool intersect(Vector3d& ray_orig, Vector3d& ray_dir, double &t) = 0;
}
