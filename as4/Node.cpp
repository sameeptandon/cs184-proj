#include "Node.h"
#include "Box.h"

using namespace std;

bool compareX( Shape* a, Shape* b ) { return ((*a).box().parameters[0](0) < (*b).box().parameters[0](0));}
bool compareY( Shape* a, Shape* b ) { return ((*a).box().parameters[0](1) < (*b).box().parameters[0](1));}
bool compareZ( Shape* a, Shape* b ) { return ((*a).box().parameters[0](2) < (*b).box().parameters[0](2));}

Node::Node(vector<Shape*> shapes, int depth) {
  _depth = depth;
  _shapes = shapes;

  vector<Shape*>::iterator it;

  double xmin = DBL_MAX, ymin = DBL_MAX, zmin = DBL_MAX;
  double xmax = DBL_MIN, ymax = DBL_MIN, zmax = DBL_MIN;
  for( it = _shapes.begin(); it != _shapes.end(); it++) {
    xmin = min( xmin, (*it)->box().parameters[0](0) );
    ymin = min( ymin, (*it)->box().parameters[0](1) );
    zmin = min( zmin, (*it)->box().parameters[0](2) );
    xmax = max( xmax, (*it)->box().parameters[1](0) );
    ymax = max( ymax, (*it)->box().parameters[1](1) );
    zmax = max( zmax, (*it)->box().parameters[1](2) );
  }

  _bb = Box(Vector3d(xmin, ymin, zmin), Vector3d(xmax, ymax, zmax));
 
  if(_shapes.size() == 1) {
    leaf = true;
    left = NULL;
    right = NULL;
    return;
  }
  else {
    leaf = false;
    int dim = depth % DIMENSIONS;
    vector<Shape*> lshapes, rshapes;
    switch(dim) {
      case 0: // x
        sort(_shapes.begin(), _shapes.end(), compareX);
        break;
      case 1: // y
        sort(_shapes.begin(), _shapes.end(), compareY);
        break;
      case 2: // z
        sort(_shapes.begin(), _shapes.end(), compareZ);
        break;
      default:
        break;
    }
    int half = _shapes.size()/2;
    lshapes.resize(half);
    rshapes.resize(_shapes.size() - half);
    copy(_shapes.begin(), _shapes.begin() + half, lshapes.begin());
    copy(_shapes.begin() + half, _shapes.end(), rshapes.begin());
    left = new Node(lshapes, depth+1);
    right = new Node(rshapes, depth+1);
  }
}

bool Node::intersect(Ray& ray, double&t, Shape* ignore, Shape** shape) {
  if( leaf ) {
    if( _shapes[0] == ignore ) return false;
    else if( _shapes[0]->intersect(ray, t) ) {
      *shape = _shapes[0];
      return true;
    }
    return false;
  }

  bool l = false;
  bool r = false;
  if(!_bb.intersect(ray, 0, DBL_MAX)) {
    return false;
  }
  else{
    double t1, t2;
    Shape *s1;
    Shape *s2;
    l = left->intersect(ray, t1, ignore, &s1);
    r = right->intersect(ray, t2, ignore, &s2);
    if( l && r ) {
      if( t1 < t2 ) {
        t = t1;
        *shape = s1;
      }
      else {
        t = t2;
        *shape = s2;
      }
      return true;
    }
    else if( l ) {
      t = t1;
      *shape = s1;
      return true;
    }
    else if( r ) {
      t = t2;
      *shape = s2;
      return true;
    }
    else {
      return false;
    }
  }
}
