#ifndef _BOX_H_
#define _BOX_H_

#include <assert.h>
#include "Globals.h"
#include "Ray.h"

/*
 * Code adapted from: http://jgt.akpeters.com/papers/WilliamsEtAl05/
 */

class Box {
  public:
    // corners
    Vector3d parameters[2];
    Box() { }
    Box(const Vector3d &min, const Vector3d &max) {
      assert(min(0) <= max(0) &&
          min(1) <= max(1) &&
          min(2) <= max(2)
          );
      parameters[0] = min;
      parameters[1] = max;
    }
    // (t0, t1) is the interval for valid hits
    bool intersect(const Ray &, double t0, double t1) const;

};

#endif // _BOX_H_
