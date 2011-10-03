#ifndef _SCENE_H
#define _SCENE_H

#include "Ray.h"
#include "Shape.h"
#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include <vector>

using namespace std;

class Scene {
  private:
    vector<Shape*> _shapes;
    //vector<Light*> _lights;
    vector<PointLight*> _point_lights;
    vector<DirectionalLight*> _directional_lights;
    //AABB tree
    //BSP
  public:
    Scene(vector<Shape*>&, vector<PointLight*>&, vector<DirectionalLight*>);
    bool intersect(Ray&, double&, Shape**);
    //inline void getLights(vector<Light*>& lights) {lights = _lights;}
    inline void getPointLights(vector<PointLight*>& point_lights) {point_lights = _point_lights;}
    inline void getDirectionalLights(vector<DirectionalLight*>& directional_lights) {directional_lights = _directional_lights;}
    
};
#endif //_SCENE_H
