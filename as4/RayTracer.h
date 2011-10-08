#ifndef _RAYTRACER_H
#define _RAYTRACER_H
#include "Ray.h"
#include "Scene.h"
#include "Viewport.h"
#include "Camera.h"
#include <vector>
#include <queue>
#include <string>

class RayTracer {
  private:
    Scene _scene;
    Camera _camera;
    queue<Ray> rayQueue;
    vector< vector <Vector3d> > pixel_colors;
    vector<PointLight*> pl;
    vector<DirectionalLight*> dl; 
    Viewport v;
    int _max_depth;
    bool _writefile;
    char *_filename;

    void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b);
  public:
    RayTracer(Scene &scene, Camera &camera, int depth, bool writefile, char* filename);
    void generateRays();
    void traceRay(Ray&);

};
#endif //_RAYTRACER_H
