#ifndef _RAY_H
#define _RAY_H
#include <Eigen/Core>
#include <Eigen/Geometry>

USING_PART_OF_NAMESPACE_EIGEN

class Shape; //Forward Declaration

class Ray {
  public:
    Ray() {};
    Ray(Vector2d, Vector3d, Vector3d, int);
    Ray(Vector2d, Vector3d, Vector3d, int, Vector3d);
    Ray(Vector2d, Vector3d, Vector3d, int, Vector3d, Shape*);
    Ray(Vector2d, Vector3d, Vector3d, int, Shape*);
    ~Ray();
    inline void getPixel(Vector2d& pix) {pix = _pixel;}
    inline void getOrigin(Vector3d& origin) {origin = _origin;}
    inline void getDirection(Vector3d& dir) {dir = _direction;}
    inline void getScale(Vector3d& scale) {scale = _scale;}
    inline int getDepth() {return _depth;}
    inline Shape* getIgnoreShape() { return _ignore_shape; }
  private:
    Vector2d _pixel;
    Vector3d _origin, _direction;
    int _depth;
    // Scale color because it's reflector ray
    Vector3d _scale;
    Shape* _ignore_shape;
};

#endif //_RAY_H
