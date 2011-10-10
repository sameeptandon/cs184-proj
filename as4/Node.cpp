#include "Node.h"
#include "Box.h"

using namespace std;

bool compareX( Shape* a, Shape* b ) { return ((*a).box().parameters[0](0) < (*b).box().parameters[0](0));}
bool compareY( Shape* a, Shape* b ) { return ((*a).box().parameters[0](1) < (*b).box().parameters[0](1));}
bool compareZ( Shape* a, Shape* b ) { return ((*a).box().parameters[0](2) < (*b).box().parameters[0](2));}

Node::Node(vector<Shape*> shapes, int depth) {
  _depth = depth;
  _shapes = shapes;

  if(_shapes.size() ==  1) {
    leaf = true;
    left = NULL;
    right = NULL;
    return;
  }
  else {
    leaf = false;
    int dim = depth % DIMENSIONS;
    vector<Shape*> lshapes, rshapes;
    vector<Shape*>::iterator it;
    switch(dim) {
      case 0: // x
        sort(_shapes.begin(), _shapes.end(), compareX);
        for( it = _shapes.begin(); it != _shapes.begin()+_shapes.size()/2; it++) {
          lshapes.push_back(*it);
        }
        for( ; it != _shapes.end(); it++) {
          rshapes.push_back(*it);
        }
        break;
      case 1: // y
        sort(_shapes.begin(), _shapes.end(), compareY);
        for( it = _shapes.begin(); it != _shapes.begin()+_shapes.size()/2; it++) {
          lshapes.push_back(*it);
        }
        for( ; it != _shapes.end(); it++) {
          rshapes.push_back(*it);
        }
        break;
      case 2: // z
        sort(_shapes.begin(), _shapes.end(), compareZ);
        for( it = _shapes.begin(); it != _shapes.begin()+_shapes.size()/2; it++) {
          lshapes.push_back(*it);
        }
        for( ; it != _shapes.end(); it++) {
          rshapes.push_back(*it);
        }
        break;
      default:
        break;
    }
    left = new Node(lshapes, depth+1);
    right = new Node(rshapes, depth+1);
  }
}
