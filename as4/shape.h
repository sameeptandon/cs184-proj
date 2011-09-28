/*
 *  Description:  Basic shape abstract class, defines the methods that shapes will have
*/
#include "ray.h"

class shape {
  protected:
    double ka, ks, kd, km;
  public:
    virtual Vector3d normal(Vector3d point) = 0;
    virtual bool intersect(ray& ray, double &t) = 0;
};
