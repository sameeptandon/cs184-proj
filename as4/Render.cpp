#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <queue>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "Sphere.h"
#include "Shape.h"
#include "Ray.h"
#include "RayTracer.h"
#include "Camera.h"
#include "Scene.h"
#include "Viewport.h"

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

  Vector3d ka = Vector3d(0.0, 0.0, 0.1);
  Vector3d kd = Vector3d(0.2, 0.5, 0.2);
  Vector3d ks = Vector3d(0.2, 0.4, 0.6);
  Vector3d km = Vector3d(0.1, 0.1, 0.1);
  double sp = 20;

  vector<Shape*> shapes;

  Sphere* s1 = new Sphere(ka, kd, ks, km, sp, Vector3d(0.75, 0.0, -2.0), 0.5);
  Sphere* s2 = new Sphere(ka, kd, ks, km, sp, Vector3d(0.55, 0.0, -4.0), 0.5);
  Ray r = Ray(Vector2d(0,0), Vector3d(0.0,0.0,0.0), Vector3d(0.0,0.0,-1.0), 0); 
  shapes.push_back(s1);
  shapes.push_back(s2);

  vector<PointLight*> point_lights;
  PointLight* l1 = new PointLight(Vector3d(5.0,0.0,2.0), Vector3d(1.0, 1.0, 1.0));
  point_lights.push_back(l1);

  vector<DirectionalLight*> directional_lights;
  DirectionalLight* l1 = new DirectionalLight(Vector3d(1.0, 0.0, 0.0), Vector3d(1.0, 1.0, 1.0));
  directional_lights.push_back(l1);

  Scene sc = Scene(shapes, point_lights, directional_lights);
  Camera cam = Camera(viewport, r);
  RayTracer rt = RayTracer(sc, cam);
  rt.rayTrace();
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
