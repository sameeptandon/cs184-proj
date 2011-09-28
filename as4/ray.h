#include <Eigen/Core>
#include <Eigen/Geometry>

USING_PART_OF_NAMESPACE_EIGEN

class ray {
  public:
    ray(Vector2d, Vector3d, Vector3d, int);
    ~ray();
    inline void getPixel(Vector2d& pix) {pix = _pixel;}
    inline void getOrigin(Vector3d& origin) {origin = _origin;}
    inline void getDirection(Vector3d& dir) {dir = _direction;}
  private:
    Vector2d _pixel;
    Vector3d _origin, _direction;
    int _depth;
};
