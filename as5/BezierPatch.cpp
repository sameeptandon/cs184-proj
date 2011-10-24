#include "BezierPatch.h"

bool BezierPatch::AddPoint(Vector3d &point) {
  controlPointsUV[x][y] = point;
  controlPointsVU[y][x] = point;
  x++;
  if(x == 4) {
    x = 0;
    y++;
  }
  if( y >= 4 )
    return false;
  return true;
}

void BezierPatch::Draw() {
  for( int i = 0; i < 4; i++ ) {
    glBegin(GL_LINE_STRIP);
    for( int j = 0; j < 4; j++ ) {
      glVertex3d(controlPointsUV[i][j][0], controlPointsUV[i][j][1], controlPointsUV[i][j][2]);
    }
    glEnd();
  }
}

// Blatantly plagiarized from: http://www-inst.eecs.berkeley.edu/~cs184/fa11/resources/bezier_notes.pdf
// given the control points of a bezier curve
// and a parametric value, return the curve 
// point and derivative
void BezierPatch::CurveInterp(vector<Vector3d> curve, double u, Vector3d &p, Vector3d &dPdu) {
  // first, split each of the three segments
  // to form two new ones AB and BC
  Vector3d A = curve[0] * (1.0-u) + curve[1] * u;
  Vector3d B = curve[1] * (1.0-u) + curve[2] * u;
  Vector3d C = curve[2] * (1.0-u) + curve[3] * u;
  // now, split AB and BC to form a new segment DE
  Vector3d D = A * (1.0-u) + B * u;
  Vector3d E = B * (1.0-u) + C * u;
  // finally, pick the right point on DE,
  // this is the point on the curve
  p = D * (1.0-u) + E * u;
  // compute derivative also
  dPdu = 3 * (E - D);
}

// given a control patch and (u,v) values, find 
// the surface point and normal
void BezierPatch::PatchInterp(double u, double v, Vector3d &p, Vector3d &n) {
  vector<Vector3d> vcurve = vector<Vector3d>(4);
  vector<Vector3d> ucurve = vector<Vector3d>(4);

  Vector3d utang, vtang; // you are trash... for now
  // build control points for a Bezier curve in v
  for( int i = 0; i < 4; i++ ) {
    CurveInterp(controlPointsUV[i], u, vcurve[i], utang);
    CurveInterp(controlPointsVU[i], v, ucurve[i], vtang);
  }

  Vector3d upt, vpt;
  // evaluate surface and derivative for u and v
  CurveInterp(vcurve, v, vpt, vtang);
  CurveInterp(ucurve, u, upt, utang);
  // take cross product of partials to find normal
  n = utang.cross(vtang).normalized(); 
  assert((upt-vpt).norm() < 0.001);
  p = upt;
}

// given a patch, perform uniform subdivision
void BezierPatch::UniformSubdivide(double step) {
  // compute how many subdivisions there 
  // are for this step size
  glBegin(GL_POLYGON);
  double epsilon = 0.001;
  int numdiv = ( (1.0 + epsilon) / step);

  Vector3d ptll, normalll;
  Vector3d ptlr, normallr;
  Vector3d ptul, normalul;
  Vector3d ptur, normalur;
  PatchInterp(0, 0, ptll, normalll);
  PatchInterp(step, 0, ptlr, normallr);
  glNormal3d(normalll(0), normalll(1), normalll(2));
  glVertex3d(ptll(0), ptll(1), ptll(2));
  glNormal3d(normallr(0), normallr(1), normallr(2));
  glVertex3d(ptlr(0), ptlr(1), ptlr(2));

  // for each parametric value of u
  for (int iu = 0; iu < numdiv-1; iu++) {
    glBegin(GL_QUAD_STRIP);
    double u = iu * step;
    double upp = (iu+1) * step;

    // for each parametric value of v
    for (int iv = 0; iv < numdiv-1; iv++) {
      double v = iv * step;
      double vpp = (iv+1) * step;

      // evaluate surface
      PatchInterp(u, vpp, ptul, normalul);
      PatchInterp(upp, vpp, ptur, normalur);
      glNormal3d(normalul(0), normalul(1), normalul(2));
      glVertex3d(ptul(0), ptul(1), ptul(2));
      glNormal3d(normalur(0), normalur(1), normalur(2));
      glVertex3d(ptur(0), ptur(1), ptur(2));
      ptll = ptul;
      ptlr = ptur;
      normalll = normalul;
      normallr = normalur;
    }
    glEnd();
  }

}
