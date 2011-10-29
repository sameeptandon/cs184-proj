#ifndef _BEZIERPATCH_H_
#define _BEZIERPATCH_H_

#include "Globals.h"

class BezierPatch {
  public:
    vector< vector<Vector3d> > controlPointsUV; 
    vector< vector<Vector3d> > controlPointsVU; 
    BezierPatch() {
      for( int i = 0; i < 4; i++ ) {
        vector<Vector3d> v;
        for( int j = 0; j < 4; j++ ) {
          v.push_back(Vector3d());
        }
        controlPointsUV.push_back(v);
        controlPointsVU.push_back(v);
      }
      x = 0;
      y = 0;
    }
    bool AddPoint(Vector3d &point);
    void Draw();
    void UniformSubdivide(double step);
    void PatchInterp(double u, double v, Vector3d &p, Vector3d &n);
    void CurveInterp(vector<Vector3d> curve, double u, Vector3d &p, Vector3d &dPdu);
    void AdaptiveSubdivideHelper(double tau, Vector2d &u1, Vector2d &u2, Vector2d &u3);
    void AdaptiveSubdivide(double tau);
  private:
    int x;
    int y; // Keep track of control points added
};

#endif
