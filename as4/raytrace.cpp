#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <Eigen/Core>
#include <Eigen/Geometry>

//Using Eigen library
USING_PART_OF_NAMESPACE_EIGEN

#ifdef OSX
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>

using namespace std;

/* Helper functions */
class Viewport;
class Viewport {
  public:
    int w, h; // width and height
};

/*function headers */
void myReshape(int w, int h);
void processNormalKeys(unsigned char key, int x, int y);
void usage();
void initScene();

Viewport viewport;

void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, viewport.w, 0, viewport.h);
}

void processNormalKeys(unsigned char key, int x, int y) {
  if( key == 32 )
    exit(0);
}

void usage() {
  cout << "Usage is undefined" << endl;
  exit(0);
}

void initScene(){
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent
  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0,viewport.w, 0, viewport.h);
}

void myDisplay() {
  glClear(GL_COLOR_BUFFER_BIT);				// clear the color buffer
  glMatrixMode(GL_MODELVIEW);					// indicate we are specifying camera transformations
  glLoadIdentity();							// make sure transformation is "zero'd"

 
  // This should be done before any other objects are shaded
  // so that other objects go on top of it
  
  
  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}


int main(int argc, char *argv[]) {

  // Read command line arguments
  int i = 0;
  while(++i != argc) {
    /*
    if (strcmp(argv[i], "-ka")==0 && i + 3 < argc) {
      ka = Vector3d(min(1.0, atof(argv[i+1])), min(1.0, atof(argv[i+2])), min(1.0, atof(argv[i+3])));
      i += 3;
    }
    else if (strcmp(argv[i], "-kd")==0 && i + 3 < argc) {
      kd = Vector3d(min(1.0, atof(argv[i+1])), min(1.0, atof(argv[i+2])), min(1.0, atof(argv[i+3])));
      i += 3;
    }
    else if (strcmp(argv[i], "-ks")==0 && i + 3 < argc) {
      ks = Vector3d(min(1.0, atof(argv[i+1])), min(1.0, atof(argv[i+2])), min(1.0, atof(argv[i+3])));
      i += 3;
    }
    else if (strcmp(argv[i], "-sp")==0 && i + 1 < argc) {
      sp = atof(argv[i+1]);
      i += 1;
    }
    else if (strcmp(argv[i], "-pl")==0 && i + 6 < argc) {
      pl_pos.push_back( Vector3d(atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3])) );
      pl_color.push_back( Vector3d(atof(argv[i+4]), atof(argv[i+5]), atof(argv[i+6])) );
      i += 6;
    }
    else if (strcmp(argv[i], "-dl")==0 && i + 6 < argc) {
      dl_dir.push_back( Vector3d(atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3])) );
      dl_color.push_back( Vector3d(atof(argv[i+4]), atof(argv[i+5]), atof(argv[i+6])) );
      i += 6;
    }
    else if (strcmp(argv[i], "-toon")==0 && i + 1 < argc) {
      toon = true;
      toon_intervals = atoi(argv[i+1]);
      i += 1;
    }
    else if (strcmp(argv[i], "-gnd")==0) {
      gnd = true;
    }
    else if (strcmp(argv[i], "-ss")==0) {
      ss = true;
    }
    else if (strcmp(argv[i], "-ms")==0) {
      ms = true;
    }
    else {
      usage();
    }
    */
  }

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

  initScene();							// quick function to set up scene

  glutDisplayFunc(myDisplay);					// function to run when its time to draw something
  glutReshapeFunc(myReshape);					// function to run when the window gets resized
  
  glutMainLoop();							// infinite loop that will keep drawing and resizing and whatever else

  return 0; //happy time
}
