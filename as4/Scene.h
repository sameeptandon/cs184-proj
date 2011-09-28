#ifndef _SCENE_H
#define _SCENE_H

#include "Ray.h"
#include "Shape.h"
#include "Light.h"
#include <vector>

using namespace std;

class Scene {
  private:
    vector<Shape*> _shapes;
    vector<Light*> _lights;
    double _sp;
    //AABB tree
    //BSP
  public:
    Scene(vector<Shape*>&, vector<Light*>&, double);
    bool intersect(Ray&, double&, Shape**);
    inline void getLights(vector<Light*>& lights) {lights = _lights;}
};
#endif //_SCENE_H
