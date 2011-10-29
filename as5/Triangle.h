#ifndef _TRIANGLE_H
#define _TRIANGLE_H
#include "Globals.h"

class Triangle { 
  protected:
    Vector3d _a, _b, _c, _normal;
  public:
    Triangle(Vector3d, Vector3d, Vector3d);
    ~Triangle();
    Vector3d normal();
    void Draw();
};
#endif //_TRIANGLE_H
