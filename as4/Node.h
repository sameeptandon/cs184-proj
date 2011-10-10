#ifndef _NODE_H
#define _NODE_H

#include "Globals.h"
#include "Shape.h"
#include "Ray.h"
#include <vector>
#include <algorithm>
#include <cfloat>

#define DIMENSIONS 3

class Node {
  public:
    Node *left, *right;
    Node(vector<Shape*>, int);
    Node() {};
    bool intersect(Ray&, double&, Shape*, Shape**);
  private:
    int _depth;
    Box _bb;
    vector<Shape*> _shapes;
    bool leaf; //is it a leaf node? i.e. only contains one shape
};
    
bool compareX( Shape* a, Shape* b );
bool compareY( Shape* a, Shape* b );
bool compareZ( Shape* a, Shape* b );

#endif //_NODE_H
