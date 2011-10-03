#ifndef _CAMERA_H
#define _CAMERA_H

#include "Viewport.h"
#include "Ray.h"

#ifdef OSX
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <Eigen/Core>
#include <Eigen/Geometry>

USING_PART_OF_NAMESPACE_EIGEN

#define BOX_PARAM 2.0

class Camera {
  private:
    Viewport _viewport;
    Ray _location;
    int _x, _y;
  public:
    Camera(Viewport&, Ray&);
    bool generateSample(Ray&);
    inline void getViewport(Viewport &viewport) {viewport = _viewport;}
};

#endif //_CAMERA_H
