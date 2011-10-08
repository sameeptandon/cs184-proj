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
#include "Triangle.h"
#include "Ellipsoid.h"
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

// Global variables
enum filetype_t {ELLIPSOID, OBJ, LIGHT};
int depth = 0;
int aasamples = 1;
int ex = 1;
bool writefile = false;
char outputfile[255];

Ray camr = Ray(Vector2d(0,0), Vector3d(0.0,0.0,0.0), Vector3d(0.0,0.0,-3.0), 0); 
vector<Shape*> shapes;
vector<PointLight*> point_lights;
vector<DirectionalLight*> directional_lights;
vector<Vector3d> vertices;

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

  double sp = 50;

  Sphere *s1, *s2, *s3, *s4, *s5;
  Ellipsoid *e1, *e2, *e3, *e4, *e5;
  Triangle *t1, *t2, *t3;
  DirectionalLight *dl1, *dl2;

  switch(ex)
  {
    case 1:
      s1 = new Sphere(
          Vector3d(0.1, 0.1, 0.1)/2, // ka 
          Vector3d(1.0, 0.0, 0.0), // kd
          Vector3d(1.0, 1.0, 1.0), // ks
          Vector3d(0.9, 0.9, 0.9), // km
          sp, Vector3d(0.0, 0.0, -17.0), 2.0);
      s2 = new Sphere(
          Vector3d(0.1, 0.1, 0.1)/2, // ka 
          Vector3d(0.0, 1.0, 0.0), // kd
          Vector3d(1.0, 1.0, 1.0), // ks
          Vector3d(0.9, 0.9, 0.9), // km
          sp, Vector3d(0.0, 4.0, -17.0), 1.5);
      s3 = new Sphere(
          Vector3d(0.1, 0.1, 0.1)/2, // ka 
          Vector3d(0.0, 0.0, 1.0), // kd
          Vector3d(1.0, 1.0, 1.0), // ks
          Vector3d(0.9, 0.9, 0.9), // km
          sp, Vector3d(0.0, -4.0, -17.0), 1.5);
      s4 = new Sphere(
          Vector3d(0.1, 0.1, 0.1)/2, // ka 
          Vector3d(1.0, 1.0, 0.0), // kd
          Vector3d(1.0, 1.0, 1.0), // ks
          Vector3d(0.9, 0.9, 0.9), // km
          sp, Vector3d(4.0, 0.0, -17.0), 1.5);
      s5 = new Sphere(
          Vector3d(0.1, 0.1, 0.1)/2, // ka 
          Vector3d(0.0, 1.0, 1.0), // kd
          Vector3d(1.0, 1.0, 1.0), // ks
          Vector3d(0.9, 0.9, 0.9), // km
          sp, Vector3d(-4.0, 0.0, -17.0), 1.5);
      shapes.push_back(s1);
      shapes.push_back(s2);
      shapes.push_back(s3);
      shapes.push_back(s4);
      shapes.push_back(s5);

      dl1 = new DirectionalLight(Vector3d(1.0, -1.0, -1.0), Vector3d(1.0, 1.0, 1.0));
      dl2 = new DirectionalLight(Vector3d(-1.0, 1.0, 1.0), Vector3d(1.0, 1.0, 1.0));
      directional_lights.push_back(dl1);
      directional_lights.push_back(dl2);
      break;
    case 2:
      s1 = new Sphere(
          Vector3d(0.1,0.1,0.1)/2, // ka 
          Vector3d(1.0, 0.0, 0.0), // kd
          Vector3d(1.0, 1.0, 1.0), // ks
          Vector3d(0.1, 0.1, 0.1), // km
          sp, Vector3d(0.0, 0.0, -20.0), 3.0);
      s2 = new Sphere(
          Vector3d(0.1,0.1,0.1)/2, // ka 
          Vector3d(1.0, 1.0, 0.0), // kd
          Vector3d(1.0, 1.0, 1.0), // ks
          Vector3d(0.0, 0.0, 0.0), // km
          sp, Vector3d(2.0, -2.0, -15.0), 1.0);
      s3 = new Sphere(
          Vector3d(0.1,0.1,0.1)/2, // ka 
          Vector3d(0.0, 1.0, 1.0), // kd
          Vector3d(1.0, 1.0, 1.0), // ks
          Vector3d(0.0, 0.0, 0.0), // km
          sp, Vector3d(-2.0, -2.0, -15.0), 1.0);
      t3 = new Triangle(
          Vector3d(0.1, 0.1, 0.1)/2, // ka
          Vector3d(0.1, 0.1, 0.1), // kd
          Vector3d(1.0, 1.0, 1.0), // ks
          Vector3d(1.0, 1.0, 1.0), // km
          sp, 
          Vector3d(5.0, 5.0, -17.0), //v1 
          Vector3d(1.0, 4.0, -20.0), //v2
          Vector3d(6.0, -1.0, -20.0)  //v3
          );
      t1 = new Triangle(
          Vector3d(0.1, 0.1, 0.1)/2, // ka
          Vector3d(0.1, 0.1, 0.1), // kd
          Vector3d(1.0, 1.0, 1.0), // ks
          Vector3d(1.0, 1.0, 1.0), // km
          sp, 
          Vector3d(15.0, -3.0, -30.0), //v1 
          Vector3d(-15.0, -3.0, -30.0), //v2
          Vector3d(-15.0, -3.0, -5.0)  //v3
          );
      t2 = new Triangle(
          Vector3d(0.1, 0.1, 0.1)/2, // ka
          Vector3d(0.1, 0.1, 0.1), // kd
          Vector3d(1.0, 1.0, 1.0), // ks
          Vector3d(1.0, 1.0, 1.0), // km
          sp, 
          Vector3d(15.0, -3.0, -5.0), //v2
          Vector3d(15.0, -3.0, -30.0), //v1 
          Vector3d(-15.0, -3.0, -5.0)  //v3
          );
      shapes.push_back(s1);
      shapes.push_back(s2);
      shapes.push_back(s3);
      shapes.push_back(t1);
      shapes.push_back(t2);
      shapes.push_back(t3);
      
      dl1 = new DirectionalLight(Vector3d(1.0, -1.0, -1.0), Vector3d(1.0, 1.0, 1.0));
      dl2 = new DirectionalLight(Vector3d(-1.0, 1.0, 1.0), Vector3d(1.0, 1.0, 1.0));
      directional_lights.push_back(dl1);
      directional_lights.push_back(dl2);
      break;
  case 3:
      e1 = new Ellipsoid(
          Vector3d(0.1, 0.1, 0.1)/2, //ka
          Vector3d(1.0, 0.0, 0.0), //kd
          Vector3d(1.0, 1.0, 1.0), //ks
          Vector3d(0.9, 0.9, 0.9), //km
          sp, //sp
          Vector3d(4.0, 2.0, 2.0), //scale
          Vector3d(0.0, 0.0, -17.0), //translation
          Vector3d(0.0, 0.0, 0.0) //rotation
          );

      e2 = new Ellipsoid(
          Vector3d(0.1, 0.1, 0.1)/2, //ka
          Vector3d(0.0, 1.0, 0.0), //kd
          Vector3d(1.0, 1.0, 1.0), //ks
          Vector3d(0.9, 0.9, 0.9), //km
          sp, //sp
          Vector3d(0.5, 1.5, 1.0), //scale
          Vector3d(-2.0, 4.0, -17.0), //translation
          Vector3d(-M_PI/4.0, -M_PI/4.0, 0.0) //rotation
          );
      e3 = new Ellipsoid(
          Vector3d(0.1, 0.1, 0.1)/2, //ka
          Vector3d(0.0, 0.0, 1.0), //kd
          Vector3d(1.0, 1.0, 1.0), //ks
          Vector3d(0.9, 0.9, 0.9), //km
          sp, //sp
          Vector3d(0.5, 1.5, 1.0), //scale
          Vector3d(-2.0, -4.0, -17.0), //translation
          Vector3d(M_PI/4.0, -M_PI/4.0, 0.0) //rotation
          );
      e4 = new Ellipsoid(
          Vector3d(0.1, 0.1, 0.1)/2, //ka
          Vector3d(1.0, 1.0, 0.0), //kd
          Vector3d(1.0, 1.0, 1.0), //ks
          Vector3d(0.9, 0.9, 0.9), //km
          sp, //sp
          Vector3d(0.5, 1.5, 1.0), //scale
          Vector3d(2.0, 4.0, -17.0), //translation
          Vector3d(-3.0*M_PI/4.0, M_PI/4.0, 0.0) //rotation
          );
      e5 = new Ellipsoid(
          Vector3d(0.1, 0.1, 0.1)/2, //ka
          Vector3d(0.0, 1.0, 1.0), //kd
          Vector3d(1.0, 1.0, 1.0), //ks
          Vector3d(0.9, 0.9, 0.9), //km
          sp, //sp
          Vector3d(0.5, 1.5, 1.0), //scale
          Vector3d(2.0, -4.0, -17.0), //translation
          Vector3d(3.0*M_PI/4.0, M_PI/4.0, 0.0) //rotation
          );

      shapes.push_back(e1);
      shapes.push_back(e2);
      shapes.push_back(e3);
      shapes.push_back(e4);
      shapes.push_back(e5);
      
      dl1 = new DirectionalLight(Vector3d(1.0, -1.0, -1.0), Vector3d(1.0, 1.0, 1.0));
      dl2 = new DirectionalLight(Vector3d(-1.0, 1.0, 1.0), Vector3d(1.0, 1.0, 1.0));
      directional_lights.push_back(dl1);
      directional_lights.push_back(dl2);
      break;
  default:
      break;
  }


  //PointLight* pl1 = new PointLight(Vector3d(0.0,0.0,-0.0), Vector3d(1.0, 1.0, 1.0));
  //point_lights.push_back(pl1);

  /*dl1 = new DirectionalLight(Vector3d(1.0, -1.0, -1.0), Vector3d(1.0, 1.0, 1.0));
  dl2 = new DirectionalLight(Vector3d(-1.0, -1.0, -1.0), Vector3d(1.0, 0.0, 0.0));
  directional_lights.push_back(dl1);
  directional_lights.push_back(dl2);
  */
  cout << "Loaded " << directional_lights.size() << " Directional Lights" << endl;
  cout << "Loaded " << point_lights.size() << " Point Lights" << endl;
  cout << "Loaded " << shapes.size() << " Shapes" << endl;
  Scene sc = Scene(shapes, point_lights, directional_lights);
  Camera cam = Camera(viewport, camr, aasamples);
  RayTracer rt = RayTracer(sc, cam, depth, writefile, outputfile);
  rt.generateRays();
  // This should be done before any other objects are shaded
  // so that other objects go on top of it
 
  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}

void parseEllipsoid(ifstream &is, char c) {
  switch(c) {
    case 'e':
      double ka_r, ka_g, ka_b;
      double ks_r, ks_g, ks_b;
      double kd_r, kd_g, kd_b;
      double km_r, km_g, km_b;
      double sp;
      double scale_x, scale_y, scale_z;
      double trans_x, trans_y, trans_z;
      double rot_x, rot_y, rot_z;
      is >> ka_r >> ka_g >> ka_b; 
      is >> ks_r >> ks_g >> ks_b; 
      is >> kd_r >> kd_g >> kd_b; 
      is >> km_r >> km_g >> km_b; 
      is >> sp;
      is >> scale_x>> scale_y>> scale_z;
      is >> trans_x>> trans_y>> trans_z;
      is >> rot_x>> rot_y>> rot_z;
      /*
      cout << " " << ka_r << " " << ka_g << " " << ka_b << endl; 
      cout << " " << ks_r << " " << ks_g << " " << ks_b << endl; 
      cout << " " << kd_r << " " << kd_g << " " << kd_b << endl; 
      cout << " " << km_r << " " << km_g << " " << km_b << endl; 
      cout << " " << sp << endl;
      cout << " " << scale_x<< " " << scale_y<< " " << scale_z << endl;
      cout << " " << trans_x<< " " << trans_y<< " " << trans_z << endl;
      cout << " " << rot_x<< " " << rot_y<< " " << rot_z << endl; 
      */
      rot_x = rot_x * M_PI/180.0;
      rot_y = rot_y * M_PI/180.0;
      rot_z = rot_z * M_PI/180.0;

      shapes.push_back(new Ellipsoid(
            Vector3d(ka_r, ka_g, ka_b),
            Vector3d(ks_r, ks_g, ks_b),
            Vector3d(kd_r, kd_g, kd_b),
            Vector3d(km_r, km_g, km_b),
            sp,
            Vector3d(scale_x, scale_y, scale_z),
            Vector3d(trans_x, trans_y, trans_z),
            Vector3d(rot_x, rot_y, rot_z)
            ));
      break;
    default:
      cout << "Every line has to begin with 'e'" << endl;
      exit(1);
      break;
  }
}

void parseObj(ifstream &is, char c) {
  double x, y, z;
  int i1, i2, i3;
  switch(c) {
    case 'v':
      is >> x >> y >> z;
      vertices.push_back(Vector3d(x,y,z));
    break;
    case 'f':
      is >> i1 >> i2 >> i3;
      shapes.push_back(new Triangle(
            Vector3d(0.1, 0.1, 0.1),
            Vector3d(1.0, 0, 0),
            Vector3d(1.0, 0, 0),
            Vector3d(0.0, 0.0, 0.0),
            20.0,
            vertices[i1-1],
            vertices[i2-1],
            vertices[i3-1]
            ));
    break;
  }
}

void parseLight(ifstream &is, char c) {
  double x,y,z;
  double r,g,b;
  switch(c) {
    case 'd':
      is >> x >> y >> z;
      is >> r >> g >> b;
      directional_lights.push_back(new DirectionalLight(
            Vector3d(x,y,z),
            Vector3d(r,g,b)));
      break;
    case 'p':
      is >> x >> y >> z;
      is >> r >> g >> b;
      point_lights.push_back(new PointLight(
            Vector3d(x,y,z),
            Vector3d(r,g,b)));
      break;
    default:
      break;
  }
}

/*
 * Code adapted from: http://www-inst.eecs.berkeley.edu/~cs184/fa11/resources/sec_TextParsing.pdf 
 */
void parseScene(char *filename, filetype_t filetype) {
  char line[1024];
  ifstream inFile(filename, ifstream::in);
  if(!inFile) {
    cout << "Could not open given file name" << endl;
    exit(1);
  }
  int i = 0;
  while(true) {
    char c;
    inFile >> c;
    if(!inFile.good()) break;
    switch(filetype) {
      case ELLIPSOID:
        parseEllipsoid(inFile, c);
        break;
      case OBJ:
        parseObj(inFile, c);
        break;
      case LIGHT:
        parseLight(inFile, c);
        break;
    }
  }
  inFile.close();
}

int main(int argc, char *argv[]) {

  // Read command line arguments
  int i = 0;
  while(++i != argc) {
    // Examples
    if (strcmp(argv[i], "-ex")==0 && i + 1 < argc) {
      ex = atoi(argv[i+1]);
      i += 1;
    }
    // .obj file
    else if (strcmp(argv[i], "-obj")==0 && i + 1 < argc) {
      filetype_t type = OBJ;
      parseScene(argv[i+1], type);
      i += 1;
    }
    // ellipsoid file
    else if (strcmp(argv[i], "-ell")==0 && i + 1 < argc) {
      filetype_t type = ELLIPSOID;
      parseScene(argv[i+1], type);
      i += 1;
    }
    // lights file
    else if (strcmp(argv[i], "-l")==0 && i + 1 < argc) {
      filetype_t type = LIGHT;
      parseScene(argv[i+1], type);
      i += 1;
    }
    // output file
    else if (strcmp(argv[i], "-o")==0 && i + 1 < argc) {
      strncpy(outputfile, argv[i+1], 255);
      outputfile[255] = '\0';
      cout << outputfile << endl;
      writefile = true;
      i += 1;
    }
    // anti-aliasing
    else if (strcmp(argv[i], "-aa")==0 && i + 1 < argc) {
      aasamples = atoi(argv[i+1]);
      i += 1;
    }
    // reflection depth
    else if (strcmp(argv[i], "-ref")==0 && i + 1 < argc) {
      depth = atoi(argv[i+1]);
      i += 1;
    }
    // camera location
    else if (strcmp(argv[i], "-cam")==0 && i + 6 < argc) {
      camr = Ray(Vector2d(0,0),
          Vector3d(atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3])),
          Vector3d(atof(argv[i+4]), atof(argv[i+5]), atof(argv[i+6])), 0);
      i += 6;
    }
    else {
      usage();
    }
  }

  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = 1000;
  viewport.h = 1000;

  //The size and position of the window
  if( !writefile ) {
    glutInitWindowSize(viewport.w, viewport.h);
    glutInitWindowPosition(0,0);
    glutCreateWindow(argv[0]);
    //Exit on spacebar
    glutKeyboardFunc(processNormalKeys);

    initScene();							// quick function to set up scene

    glutDisplayFunc(myDisplay);					// function to run when its time to draw something
    glutReshapeFunc(myReshape);					// function to run when the window gets resized

    glutMainLoop();							// infinite loop that will keep drawing and resizing and whatever else
  }
  else {
    myDisplay();
  }
  return 0; //happy time
}
