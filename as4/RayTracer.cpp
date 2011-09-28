#include "RayTracer.h"

RayTracer::RayTracer(Scene &scene, Camera &camera ) :
  _scene(scene),
  _camera(camera) {};

void RayTracer::rayTrace() {
  Ray r;
  double t;
  Shape* s;
  Vector2d pix;
  Vector3d a,b;
  glBegin(GL_POINTS);
  while(_camera.generateSample(r)) {
    if(_scene.intersect(r, t, &s)) {
      r.getPixel(pix);
      // Do Phong shading here
      setPixel(pix(0), pix(1), 255, 0.0, 0.0);
    }
  }
  glEnd();
}

void RayTracer::setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glVertex2f(x+0.5, y+0.5);
}
