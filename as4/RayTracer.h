#ifndef _RAYTRACER_H
#define _RAYTRACER_H
#include "Ray.h"
#include "Scene.h"
#include "Viewport.h"
#include "Camera.h"

class RayTracer {
  private:
    Scene _scene;
    Camera _camera;
    void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b);
  public:
    RayTracer(Scene &scene, Camera &camera);
    void rayTrace();

};
#endif //_RAYTRACER_H
