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
    Vector3d _location, _ll, _ul, _lr;
    int _x, _y, _aa_sampling, _aa_count;
  public:
    Camera(Viewport&, Vector3d&, Vector3d&, Vector3d&, Vector3d&, int aa_sampling);
    bool generateSample(Ray&);
    inline void getViewport(Viewport &viewport) {viewport = _viewport;}
};

#endif //_CAMERA_H
