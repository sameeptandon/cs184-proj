#ifndef _GLOBALS_H
#define _GLOBALS_H
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/LU>
#include <vector>

#ifdef OSX
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif


USING_PART_OF_NAMESPACE_EIGEN
using namespace std;
#endif //_GLOBALS_H
