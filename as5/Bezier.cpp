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

static struct timeval lastTime;

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
bool save = false;
int last_x = 0;
int last_y = 0;
bool smoothShading = true;
bool wireFrame = false; 

Vector3d max_v = Vector3d(-9999999, -9999999, -999999);
Vector3d min_v = Vector3d(9999999, 99999999, 99999999); 

int rx = 0;
int ry = 0;
double tx = 0;
double ty = 0;
double tz = 1;
int pressed_mouse_button; // get mouse button state

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

  //glMatrixMode(GL_PROJECTION);					// indicate we are specifying camera transformations
  //glLoadIdentity();							// make sure transformation is "zero'd"

  //glOrtho(-5, 5, -5, 5, -5, 5);
  //glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();
  //glEnable(GL_LIGHTING); // turn on the lights, we have power!!!

  //glScaled(.25, .25, 1); // make the scene smaller so we can see the damn thing

  //glColor3f(1.0f,0.5f,0.0f);          // setting the color to orange
 
  glPushMatrix();
  glScaled(tz,tz,tz);
  glTranslated(tx, ty, 0);
  glRotated(ry, 1.0, 0.0, 0.0);
  glRotated(rx, 0.0, 1.0, 0.0);

  //glutSolidTeapot(2);
  
  for( int i = 0; i < patches.size() ; i++ ) {
    patches[i].AdaptiveSubdivide(0.01);
    //patches[i].UniformSubdivide(1.0/20.0);
    //patches[i].UniformSubdivide(1.0);
    //patches[i].Draw();
  }

  glPopMatrix();
 
  // This should be done before any other objects are shaded
  // so that other objects go on top of it
  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
  if( save ) {
    char filename[256];
    sprintf(filename, "output.png");
    save_opengl_image(viewport.w, viewport.h, filename);
    exit(0);
  }
}


/*
 * Stole code from: 
 */
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
/*
 
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT1, GL_POSITION, position1);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  glEnable(GL_LIGHT2);
  glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT2, GL_POSITION, position2);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
*/
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
  cout << "Usage is undefined";
  exit(0);
}

void processNormalKeys(unsigned char key, int x, int y) {
  if( key == 32 )
    exit(0);
  if (key == 's') 
    smoothShading = !smoothShading;
  if (key == 'w')
    wireFrame = !wireFrame; 
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
      ty += 0.25; 
    }
    if (key == GLUT_KEY_UP) { 
      ty -= 0.25;
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


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {

  parsePatches(argv[1]);
  
  /*
  // Read command line arguments
  int i = 0;
  while(++i != argc) {
    if (strcmp(argv[i], "-blah")==0) {
      cout << "blah." << endl;
    }
    else {
      usage();
    }
  }
  */

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
