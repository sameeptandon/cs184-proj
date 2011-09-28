#include <Eigen/Core>
#include <Eigen/Geometry>

USING_PART_OF_NAMESPACE_EIGEN

class ray {
  public:
    ray(Vector2d, Vector3d, Vector3d, int);
    ~ray();
  private:
    Vector2d _pixel;
    Vector3d _origin, _direction;
    int _depth;
};
