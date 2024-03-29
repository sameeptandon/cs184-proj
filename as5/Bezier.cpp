#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>

#include <time.h>
#include <math.h>

#include "Globals.h"
#include "BezierPatch.h"
#include "FileWriter.h"
#include "Triangle.h"

static struct timeval lastTime;
void writeToObj();
using namespace std;

//****************************************************
// Some Classes
//****************************************************

class Viewport;

class Viewport {
  public:
    int w, h; // width and height
};


//****************************************************
// Global Variables
//****************************************************
Viewport	viewport;
vector<BezierPatch> patches;
vector<Triangle> triangles;
vector<Triangle> trianglesOut;
bool save = false;
bool adaptive = false;
int last_x = 0;
int last_y = 0;
bool smoothShading = true;
bool wireFrame = false;
bool hiddenLineRemoval = false;
double subdivParam;
bool writeout = false;

Vector3d max_v = Vector3d(-9999999, -9999999, -999999);
Vector3d min_v = Vector3d(9999999, 99999999, 99999999); 

int rx = 0;
int ry = 0;
double tx = 0;
double ty = 0;
double tz = 1;
int pressed_mouse_button; // get mouse button state

char outfile[512];
char outputfile[256];

void initScene(){
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent

  glViewport (0,0,viewport.w,viewport.h);
  //glMatrixMode(GL_PROJECTION);
  //glLoadIdentity();
  gluOrtho2D(0,viewport.w, 0, viewport.h);
}


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;
  glViewport(0, 0, viewport.w, viewport.h);

  // set perspective viewing frustum
  float aspectRatio = (float)w / h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glFrustum(-aspectRatio, aspectRatio, -1, 1, 1, 100);
  gluPerspective(60.0f, (float)(w)/h, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip

  // switch to modelview matrix in order to set scene
  glMatrixMode(GL_MODELVIEW);

    //glMatrixMode(GL_PROJECTION);
  //glLoadIdentity();
  //gluOrtho2D(0, viewport.w, 0, viewport.h);

}

//***************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {
  if(!smoothShading) { 
    glDisable(GL_SMOOTH);
    glEnable(GL_FLAT);
    glShadeModel(GL_FLAT);
  } else {
    glDisable(GL_FLAT);
    glEnable(GL_SMOOTH);
    glShadeModel(GL_SMOOTH);
  }
  if (!wireFrame) { 
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } 

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// clear the color buffer

  glPushMatrix();
  glScaled(tz,tz,tz);
  glTranslated(tx, ty, 0);
  glRotated(-ry, 1.0, 0.0, 0.0);
  glRotated(rx, 0.0, 1.0, 0.0);

  //glutSolidTeapot(2);
  
  for( int i = 0; i < patches.size() ; i++ ) {
    if(adaptive) {
      patches[i].AdaptiveSubdivide(subdivParam);
    }
    else {
      patches[i].UniformSubdivide(subdivParam);
    }
    //patches[i].UniformSubdivide(1.0);
    //patches[i].Draw();
  }

  for (int i = 0; i < triangles.size() ; i++ ) {
    triangles[i].Draw();
  }

  if (wireFrame && hiddenLineRemoval) { //hidden line removal code

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 0.0f, 0.0f);

    for( int i = 0; i < patches.size() ; i++ ) {
      if(adaptive) {
        patches[i].AdaptiveSubdivide(subdivParam);
      }
      else {
        patches[i].UniformSubdivide(subdivParam);
      }
      //patches[i].UniformSubdivide(1.0);
      //patches[i].Draw();
    }

    for (int i = 0; i < triangles.size() ; i++ ) {
      triangles[i].Draw();
    }

    glEnable(GL_LIGHTING);
    glDisable(GL_POLYGON_OFFSET_FILL);
  }

  glPopMatrix();
 
  // This should be done before any other objects are shaded
  // so that other objects go on top of it
  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
  if( save ) {
    save_opengl_image(viewport.w, viewport.h, outputfile);
    exit(0);
  }
  if( writeout ) {
    writeToObj();
    exit(0);
  }
}


void initlights(void)
{
  GLfloat position[] = {0.0, 0.0, 20.0, 1.0};
  GLfloat position1[] = {0.0, 10.0, 0.0, 1.0};
  GLfloat position2[] = {0.0, 0.0, 10.0, 1.0};
  GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0};
  GLfloat light_diffuse[] = {0.0, 0.6, 0.6, 1.0};
  GLfloat light_specular[] = {1.0, 1.0, 0.0, 1.0};
  GLfloat mat_diffuse[] = {0.0, 0.6, 0.6, 1.0};
  GLfloat mat_specular[] = {1.0, 1.0, 0.0, 1.0};
  //GLfloat mat_shininess[] = {50.0};
  GLfloat mat_shininess[] = {50.0};

  glEnable(GL_LIGHTING);
  
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  //glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glEnable(GL_LIGHT0);
 
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT2, GL_POSITION, position1);
  //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glEnable(GL_LIGHT1);

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT2, GL_POSITION, position2);
  //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glEnable(GL_LIGHT2);
}

void parsePatch(ifstream &is) {
  double x,y,z;
  Vector3d v;
  BezierPatch bp = BezierPatch();
  do {
    is >> z >> x >> y;
    v = Vector3d(x,y,z);
    for (int j = 0; j < 3; j++) { 
      min_v(j) = min(min_v(j), v(j));
      max_v(j) = max(max_v(j), v(j));
    }
  } while( bp.AddPoint(v) );
  patches.push_back(bp);
}

void parseObj(char* filename) {
  ifstream in(filename, ifstream::in);
  if(!in) {
    cout << "Could not open given file name " << filename << endl;
    exit(1);
  }
  char c;
  double x,y,z;
  int i1, i2, i3;
  vector<Vector3d> vertices;
  Vector3d v;
  while(true) {
    in >> c;
    if(!in.good()) break;
    switch(c) {
      case 'v':
        in >> x >> y >> z;
        v = Vector3d(x,y,z);
        vertices.push_back(v);
        for (int j = 0; j < 3; j++) { 
          min_v(j) = min(min_v(j), v(j));
          max_v(j) = max(max_v(j), v(j));
        }
        // cout << "Adding vertex at " << transformed.transpose() << endl;
        break;
      case 'f':
        in >> i1 >> i2 >> i3;
        triangles.push_back(Triangle(
              vertices[i1-1],
              vertices[i2-1],
              vertices[i3-1]
              ));
        break;
    }
  }
  in.close();
}
void MouseMotion(int x, int y)
{

  rx += x - last_x;
  ry += last_y - y;

  last_x = x;
  last_y = y;
  glutPostRedisplay();

}
void processMouse(int button, int state, int x, int y) {
  if(state == GLUT_DOWN) {
    pressed_mouse_button = button;
    if (button == GLUT_LEFT_BUTTON) {
      last_x = x;
      last_y = y;
    }
  }
}

void writeToObj() {
  static bool firstTime = true;
  ofstream of (outfile, ios::out);
  
  if(firstTime) {
    trianglesOut.insert(trianglesOut.end(), triangles.begin(), triangles.end());
    for( int i = 0; i < patches.size(); i++ ) {
      trianglesOut.insert(trianglesOut.end(), patches[i].triangles.begin(), patches[i].triangles.end());
    }
  }

  for( int i = 0; i < trianglesOut.size(); i++ ) {
    of << "v " << trianglesOut[i]._a(0) << " " << trianglesOut[i]._a(1) << " " << trianglesOut[i]._a(2) << endl;
    of << "v " << trianglesOut[i]._b(0) << " " << trianglesOut[i]._b(1) << " " << trianglesOut[i]._b(2) << endl;
    of << "v " << trianglesOut[i]._c(0) << " " << trianglesOut[i]._c(1) << " " << trianglesOut[i]._c(2) << endl;
  }
  for( int i = 0; i < trianglesOut.size(); i++ ) {
    of << "f " << 3*i+1 << " " << 3*i+2 << " " << 3*i+3 << endl;
  }
  of.close();
  firstTime = false;
}

/*
 * Code adapted from: http://www-inst.eecs.berkeley.edu/~cs184/fa11/resources/sec_TextParsing.pdf 
 */
void parsePatches(char *filename) {
  ifstream inFile(filename, ifstream::in);
  if(!inFile) {
    cout << "Could not open given file name " << filename << endl;
    exit(1);
  }
  int i, n;
  inFile >> n;
  for( i = 0; i < n; i++ ) {
    if(!inFile.good()) break;
    parsePatch(inFile);
  }
  inFile.close();
}

void printPatches() {
  for( int i=0; i < patches.size(); i++ ) {
    assert(patches[i].controlPointsUV[2][1].transpose() == patches[i].controlPointsVU[1][2].transpose());
    cout << patches[i].controlPointsUV[2][1].transpose() << endl;
    cout << patches[i].controlPointsVU[1][2].transpose() << endl;
  }
}

void usage() {
  cout << "Press w to switch to/from wireframe mode" << endl;
  cout << "      s to switch between shading modes" << endl;
  cout << "      a to switch between subdivision modes" << endl;
  cout << "      +- to zoom" << endl;
  cout << "      shift+arrow keys to move the camera" << endl;
}

void processNormalKeys(unsigned char key, int x, int y) {
  int mod = glutGetModifiers();
  bool ctrl = false;
  if (mod == GLUT_ACTIVE_CTRL) {
    ctrl = true;
  }
  if (ctrl) {
    if (key+96 == 's') {
      save = true;
    }
  }
  else {
    if (key == 's') 
      smoothShading = !smoothShading;
  }
  if( key == 32 )
    exit(0);
  if (key == 'w')
    wireFrame = !wireFrame; 
  if (key == 'h')
    hiddenLineRemoval = !hiddenLineRemoval;
  if (key == 'a')
     adaptive = !adaptive; 
  if (key == '+') 
    tz *= 1.1;
  if (key == '-' || key == '_')
    tz /= 1.1;
 
  glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y) {
  int mod = glutGetModifiers();
  bool shift = false; 
  if (mod == GLUT_ACTIVE_SHIFT) {
    shift = true; 
  }

  if (shift) { 
    if (key == GLUT_KEY_LEFT) {
      tx -= 0.25;
    }
    if (key == GLUT_KEY_RIGHT) { 
      tx += 0.25;
    }
    if (key == GLUT_KEY_DOWN) { 
      ty -= 0.25; 
    }
    if (key == GLUT_KEY_UP) { 
      ty += 0.25;
    }
  }
  else {
    if (key == GLUT_KEY_LEFT) {
      rx -= 10;
    }
    if (key == GLUT_KEY_RIGHT) { 
      rx += 10;
    }
    if (key == GLUT_KEY_DOWN) { 
      ry += 10; 
    }
    if (key == GLUT_KEY_UP) { 
      ry -= 10;
    }
  }
  glutPostRedisplay();

}

void initGL()
{
  // enable /disable features
  //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_DEPTH_TEST);


  //glEnable(GL_LIGHTING);
  //glEnable(GL_TEXTURE_2D);
  //glEnable(GL_CULL_FACE);

  // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
  //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  //glEnable(GL_COLOR_MATERIAL);

  //glClearColor(0, 0, 0, 0);                   // background color
  //glClearStencil(0);                          // clear stencil buffer
  glClearDepth(1.0f);                         // 0 is near, 1 is far
  //glDepthFunc(GL_LEQUAL);
  initlights();
  
  //set camera
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, max_v(2) + 5, (max_v(0)/2) + min_v(0)/2, (max_v(1)/2) + min_v(1)/2,(max_v(2)/2) + min_v(2)/2, 0, 1, 0);

}

//Code taken from: http://stackoverflow.com/questions/1711095/parse-out-the-file-extension-from-a-file-path-in-c
int endswith(const char* haystack, const char* needle)
{
  size_t hlen;
  size_t nlen;
  /* find the length of both arguments - 
   *     if needle is longer than haystack, haystack can't end with needle */
  hlen = strlen(haystack); 
  nlen = strlen(needle);
  if(nlen > hlen) return 0;

  /* see if the end of haystack equals needle */
  return (strcmp(&haystack[hlen-nlen], needle)) == 0;
}

void help(void) {
  cout << "./bezier <.bez or .obj file> <subdiv param> -a -o <output obj>" << endl;
  exit(0);
}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
  if (argc < 2) {
    help();
  }

  if (endswith(argv[1], "bez")) {
    parsePatches(argv[1]);
  } else if (endswith(argv[1], "obj")) {
    parseObj(argv[1]);
  }
  if(argc >=3)
    subdivParam = atof(argv[2]);
  else
    subdivParam = 0.1;

  usage();
  // Read command line arguments
  int i = 2;
  while(i+1 <= argc) {
    if (strcmp(argv[i], "-a")==0) {
      adaptive = true;
    }
    else if (strcmp(argv[i], "-o")==0 && i+1 < argc) {
      strncpy(outfile, argv[i+1], 511);
      outfile[511] = '\0';
      writeout = true;
      i++;
    }
    else if (strcmp(argv[i], "-out")==0 && i + 1 < argc) {
      strncpy(outputfile, argv[i+1], 255);
      outputfile[255] = '\0';
      i += 1;
    }
    i++;
  }


  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // Initalize theviewport size
  viewport.w = 600;
  viewport.h = 600;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);

  //Exit on spacebar
  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(processSpecialKeys);
  /*
   * glutMotionFunc(MouseMotion);
  glutMouseFunc(processMouse);
  */
  initScene();							// quick function to set up scene
  initGL();
  
  glutMotionFunc(MouseMotion);
  glutMouseFunc(processMouse);
  glutDisplayFunc(myDisplay);					// function to run when its time to draw something
  glutReshapeFunc(myReshape);					// function to run when the window gets resized
  
  glutMainLoop();							// infinite loop that will keep drawing and resizing and whatever else

  return 0;
}
