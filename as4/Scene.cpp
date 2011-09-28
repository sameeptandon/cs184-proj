#include "Scene.h"
#include <cfloat>

Scene::Scene(vector<Shape*>& shapes, vector<Light*>& lights, double sp) :
  _shapes(shapes),
  _lights(lights),
  _sp(sp) {};

bool Scene::intersect(Ray& ray, double& t, Shape** shape) {
  bool intersect = false;
  t = DBL_MAX;

  for( int i = 0; i < _shapes.size(); i++ ) {
    double tmp;
    bool it = _shapes[i]->intersect(ray, tmp);
    if( it && tmp < t ) {
      intersect = true;
      t = tmp;
      *shape = _shapes[i];
    }
  }
  return intersect;
}
