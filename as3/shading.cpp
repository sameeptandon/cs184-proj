// Simple OpenGL example for CS184 F06 by Nuttapong Chentanez, modified from sample code for CS184 on Sp06
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

#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

#ifdef OSX
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>

#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define PI 3.14159265

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
int 		plotX = 0;
int 		plotY = 0;
// Some default coefficients
Vector3d ka = Vector3d(255.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f);
Vector3d kd = Vector3d(255.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f);
Vector3d ks = Vector3d(255.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f);
// Default lighting
vector<Vector3d> pl_pos, pl_color, dl_dir, dl_color;
// Default specular term power
double sp = 1.0f;
int pressed_mouse_button; // get mouse button state
double rx = 0;
double ry = 0;
int last_x = 0;
int last_y = 0;


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

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glVertex2f(x+0.5, y+0.5);
}

void shaded_sphere(int radius) {
  // Draw inner circle
  glBegin(GL_POINTS);

  for (double y = -radius; y <= radius; y++) {
    int width = (int)(sqrt((double)(radius*radius-y*y)) + 0.5f);
    for (double x = -width; x <= width; x++) {

      Vector3d pixel_color = Vector3d::Zero();

      // Calculate normal
      double z = sqrt(radius*radius - x*x - y*y);

      Vector3d normal = Vector3d(x,y,z);
      Vector3d normal_hat = normal.normalized();

      // Calculate intensity
      Vector3d intensity = Vector3d::Zero();

      assert( pl_color.size() == pl_pos.size());
      assert( dl_color.size() == dl_dir.size());

      // Loop over point lights
      for( int i = 0; i < pl_color.size(); i++ ) {
        // Diffuse light
        Vector3d i_hat_pl = ((pl_pos[i] * radius) - normal).normalized();
        double i_pl_dot_n = (i_hat_pl.dot( normal_hat ));
        Vector3d diff_pl = kd.cwise() * pl_color[i] * max(0.0, i_pl_dot_n);
        // Specular light 
        Vector3d r_pl = -i_hat_pl + 2 * i_pl_dot_n * normal_hat;
        Vector3d spec_pl = ks.cwise() * pl_color[i] * pow(max(0.0, r_pl.normalized().dot( Vector3d(0.0,0.0,1.0) )), sp);

        pixel_color += diff_pl + spec_pl;
        intensity += pl_color[i];
      }
      // Loop over directional lights
      for( int i = 0; i < dl_color.size(); i++ ) {
        // Diffuse light
        Vector3d i_hat_dl = dl_dir[i].normalized();
        double i_dl_dot_n = (i_hat_dl.dot( normal_hat ));
        Vector3d diff_dl = kd.cwise() * dl_color[i] * max(0.0, i_dl_dot_n);
        // Specular light 
        Vector3d r_dl = -i_hat_dl + 2 * i_dl_dot_n * normal_hat;
        Vector3d spec_dl = ks.cwise() * dl_color[i] * pow(max(0.0, r_dl.normalized().dot( Vector3d(0.0,0.0,1.0) )), sp);

        pixel_color += diff_dl + spec_dl;
        intensity += dl_color[i];
      }

      // Ambient light
      Vector3d amb = ka.cwise() * intensity;

      pixel_color += amb;

      // Set the red pixel
      setPixel(viewport.w/2.0 + x, viewport.h/2.0 + y, pixel_color(0), pixel_color(1), pixel_color(2));
    }
  }
  glEnd();
}
//***************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

  glClear(GL_COLOR_BUFFER_BIT);				// clear the color buffer

  glMatrixMode(GL_MODELVIEW);					// indicate we are specifying camera transformations
  glLoadIdentity();							// make sure transformation is "zero'd"

  shaded_sphere(min(viewport.w, viewport.h) / 2.5);

  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}

//****************************************************
// for updating the position of the circle
//****************************************************

void myFrameMove() {
  float dt;
  // Compute the time elapsed since the last time the scence is redrawn
#ifdef _WIN32
  DWORD currentTime = GetTickCount();
  dt = (float)(currentTime - lastTime)*0.001f; 
#else
  timeval currentTime;
  gettimeofday(&currentTime, NULL);
  dt = (float)((currentTime.tv_sec - lastTime.tv_sec) + 1e-6*(currentTime.tv_usec - lastTime.tv_usec));
#endif

  // Update the position of the circle
  static float totalTime = 0.0f;
  const float rotSpeed = 1.0f; // In one revolution per second
  float pathRadius = min(viewport.w, viewport.h) * 0.25f;  // Radius of the circular path	
  plotX = (int)(viewport.w*0.5f + pathRadius*cos(rotSpeed*PI*2.0f*totalTime));
  plotY = (int)(viewport.h*0.5f + pathRadius*sin(rotSpeed*PI*2.0f*totalTime));

  // Accumulate the time since the program starts
  totalTime += dt;

  // Store the time
  lastTime = currentTime;
  glutPostRedisplay();
}

void usage() {
  cout << "Usage is -ka r g b -kd r g b -ks r g b -sp v -pl x y z r g b -dl x y z r g b\n";
  exit(0);
}

void processNormalKeys(unsigned char key, int x, int y) {
  if( key == 32 )
    exit(0);
}

void MouseMotion(int x, int y)
{
  //rx += x - last_x;
  //ry += last_y - y;

  rx = ((x - last_x) / 180.0) * M_PI;
  ry = ((y - last_y) / 180.0) * M_PI;

  Matrix3d rot_y = Eigen::AngleAxisd(ry, Vector3d::UnitX()).toRotationMatrix();
  Matrix3d rot_x = Eigen::AngleAxisd(rx, Vector3d::UnitY()).toRotationMatrix();

  for (int i = 0; i < pl_pos.size(); i++) {
    pl_pos[i] = rot_x * rot_y * pl_pos[i];
  }

  for (int i = 0; i < dl_dir.size(); i++) {
    dl_dir[i] = rot_x * rot_y * dl_dir[i];
  }

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

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {

  // Read command line arguments
  int i = 0;
  while(++i != argc) {
    if (strcmp(argv[i], "-ka")==0 && i + 3 < argc) {
      ka = Vector3d(atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3]));
      i += 3;
    }
    else if (strcmp(argv[i], "-kd")==0 && i + 3 < argc) {
      kd = Vector3d(atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3]));
      i += 3;
    }
    else if (strcmp(argv[i], "-ks")==0 && i + 3 < argc) {
      ks = Vector3d(atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3]));
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
    else {
      usage();
    }
  }

  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = 900;
  viewport.h = 900;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);

  //Exit on spacebar
  glutKeyboardFunc(processNormalKeys);
  glutMotionFunc(MouseMotion);
  glutMouseFunc(processMouse);
  // Initialize timer variable
#ifdef _WIN32
  lastTime = GetTickCount();
#else
  gettimeofday(&lastTime, NULL);
#endif 	

  initScene();							// quick function to set up scene

  glutDisplayFunc(myDisplay);					// function to run when its time to draw something
  glutReshapeFunc(myReshape);					// function to run when the window gets resized
  // glutIdleFunc(myFrameMove);			

  glutMainLoop();							// infinite loop that will keep drawing and resizing and whatever else

  return 0;
}
