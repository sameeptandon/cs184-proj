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

int rx = 0;
int ry = 0;
int pressed_mouse_button; // get mouse button state

void initScene(){
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent

  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0,viewport.w, 0, viewport.h);
}


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, viewport.w, 0, viewport.h);

}

//***************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// clear the color buffer

  glMatrixMode(GL_PROJECTION);					// indicate we are specifying camera transformations
  glLoadIdentity();							// make sure transformation is "zero'd"

  glOrtho(-5, 5, -5, 5, -5, 5);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //glEnable(GL_LIGHTING); // turn on the lights, we have power!!!

  //glTranslatef(0, 0, -10);
  //glScaled(.25, .25, 1); // make the scene smaller so we can see the damn thing
  glRotated(ry, 1.0, 0.0, 0.0);
  glRotated(rx, 0.0, 1.0, 0.0);

  glColor3f(1.0f,0.5f,0.0f);          // setting the color to orange
 
  //glutSolidTeapot(1);
  for( int i = 0; i < patches.size() ; i++ ) {
    patches[i].UniformSubdivide(1.0/10.0);
    //patches[i].UniformSubdivide(1.0);
    //patches[i].Draw();
  }
 
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

void parsePatch(ifstream &is) {
  double x,y,z;
  Vector3d v;
  BezierPatch bp = BezierPatch();
  do {
    is >> z >> x >> y;
    v = Vector3d(x,y,z);
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
  /*
  if ( key == 'x') {
    for (int i = 0; i < pl_pos.size(); i++) {
      cout << "-pl " << pl_pos[i].transpose() << " " << pl_color[i].transpose() << " ";
    }
    cout << endl; 
  }
  */
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
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = 600;
  viewport.h = 600;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);

  //Exit on spacebar
  glutKeyboardFunc(processNormalKeys);
  /*
   * glutMotionFunc(MouseMotion);
  glutMouseFunc(processMouse);
  */
  initScene();							// quick function to set up scene

  glutMotionFunc(MouseMotion);
  glutMouseFunc(processMouse);
  glutDisplayFunc(myDisplay);					// function to run when its time to draw something
  glutReshapeFunc(myReshape);					// function to run when the window gets resized
  
  glutMainLoop();							// infinite loop that will keep drawing and resizing and whatever else

  return 0;
}
