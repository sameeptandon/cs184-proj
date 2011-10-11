#include "Scene.h"
#include <cfloat>

Scene::Scene(vector<Shape*>& shapes, vector<PointLight*>& points_lights, vector<DirectionalLight*> directional_lights, bool kdAccel) :
  _shapes(shapes),
  _point_lights(points_lights),
  _directional_lights(directional_lights),
  _kdAccel(kdAccel)
  {
    if( kdAccel ) {
      cout << "Constructing kd-tree of shapes..." << endl;
      _kdRoot = Node(shapes, 0);
    }
  };


bool Scene::intersect(Ray& ray, double& t, Shape** shape) {

  Shape* ignore = ray.getIgnoreShape();
  bool intersect = false;
  t = DBL_MAX;
  if( _kdAccel ) {
    return _kdRoot.intersect(ray, t, ignore, shape);
  }
  else {
    for( int i = 0; i < _shapes.size(); i++ ) {
      if (_shapes[i] == ignore) continue;  
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

}
