#include "shape.h"

class sphere : public shape {
  protected:
    Vector3d _center;
    int _radius;
  public:
    sphere(Vector3d, int);
    ~sphere();
    Vector3d normal(Vector3d);
    bool intersect(ray&, double&);
};
