#define BULLET 1 

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "ray_sphere_check.h"

#if BULLET
#include "bounce_sim.h"
#endif

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

#include "write_to_file.h"
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
Vector3d ka = Vector3d(0.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f);
Vector3d kd = Vector3d(0.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f);
Vector3d ks = Vector3d(0.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f);
// Default lighting
vector<Vector3d> pl_pos, pl_color, dl_dir, dl_color;
// Default specular term power
double sp = 1.0f;
// Default toon shading option
bool toon = false;
int toon_intervals = 1;
// Default ground option (no ground)
bool gnd = false;
bool ss = false;
bool ms = false;
int pressed_mouse_button; // get mouse button state
double rx = 0;
double ry = 0;
int last_x = 0;
int last_y = 0;
int fileout_count = 0;

double trans_x;
double trans_y;

typedef struct {
  int radius;
  int x_offset;
  int y_offset;
} sphere_t;

vector<sphere_t> spheres;

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

  trans_x = viewport.w / 2.0;
  trans_y = viewport.h / 2.0;
  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, viewport.w, 0, viewport.h);

}

double toon_interpolate(GLfloat val) {
  double interval = 1.0/(float) toon_intervals;
  for( double i = 0.0; i < 1.0; i += interval ) {
    if( (double) val > i && (double) val <= i + interval ) {
      return (GLfloat) i + interval/2;
    }
  }
}

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  if( !toon ) 
    glColor3f(r, g, b);
  else
    glColor3f(toon_interpolate(r), toon_interpolate(g), toon_interpolate(b));
  glVertex2f(x+0.5, y+0.5);
}

void shaded_sphere(int radius, int x_offset, int y_offset) {
  // Draw inner circle
  glBegin(GL_POINTS);

  for (int y = -radius; y <= radius; y++) {
    int width = (int)(sqrt((double)(radius*radius-y*y)) + 0.5f);
    for (int x = -width; x <= width; x++) {

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
        bool intersection = false;

        double their_t;
        Vector3d i_pl = -((pl_pos[i] * radius) + Vector3d(trans_x, trans_y, 0) - Vector3d(x_offset, y_offset,0)- normal);
#if 0
        for( int j = 0; j < spheres.size(); j++ ) {
          Vector3d pl_trans = ((pl_pos[i] * radius)) - Vector3d(spheres[j].x_offset, spheres[j].y_offset,0);
          if( spheres[j].radius == radius && spheres[j].x_offset == x_offset && spheres[j].y_offset == y_offset ) {
            continue;
          }
          else {
            if (intersect( pl_trans, i_pl, their_t, (double) radius) && their_t < 1.0) {
              //cout << "INTERSECT" << endl; 
              intersection =  true;
              break;
            }
          }

        }
#endif
        if( !intersection ) {
          // Diffuse light
          Vector3d i_hat_pl = -i_pl.normalized();
          double i_pl_dot_n = (i_hat_pl.dot( normal_hat ));
          Vector3d diff_pl = kd.cwise() * pl_color[i] * max(0.0, i_pl_dot_n);
          // Specular light 
          Vector3d r_pl = -i_hat_pl + 2 * i_pl_dot_n * normal_hat;
          Vector3d spec_pl = ks.cwise() * pl_color[i] * pow(max(0.0, r_pl.normalized().dot( Vector3d(0.0,0.0,1.0) )), sp);

          pixel_color += diff_pl + spec_pl;
          intensity += pl_color[i];
        }
      }
      // Loop over directional lights
      for( int i = 0; i < dl_color.size(); i++ ) {

        bool intersection = false;
        double their_t;
        Vector3d i_dl = dl_dir[i];
#if 0
        for( int j = 0; j < spheres.size(); j++ ) {
          Vector3d dl_pos = 2*(Vector3d(spheres[j].x_offset, spheres[j].y_offset, 0) - Vector3d(x_offset, y_offset, 0)) + normal;

          if( spheres[j].radius == radius && spheres[j].x_offset == x_offset && spheres[j].y_offset == y_offset ) {
            continue;
          }
          else {
            if (intersect(dl_pos, i_dl, their_t, (double) radius)) {
              //cout << "INTERSECT" << endl; 
              intersection =  true;
              break;
            }
          }

        }
#endif
        if( !intersection ) {
          // Diffuse light
          Vector3d i_hat_dl = -dl_dir[i].normalized();
          double i_dl_dot_n = (i_hat_dl.dot( normal_hat ));
          Vector3d diff_dl = kd.cwise() * dl_color[i] * max(0.0, i_dl_dot_n);
          // Specular light 
          Vector3d r_dl = -i_hat_dl + 2 * i_dl_dot_n * normal_hat;
          Vector3d spec_dl = ks.cwise() * dl_color[i] * pow(max(0.0, r_dl.normalized().dot( Vector3d(0.0,0.0,1.0) )), sp);

          pixel_color += diff_dl + spec_dl;
          intensity += dl_color[i];
        }
      }

      // Ambient light
      Vector3d amb = ka.cwise() * intensity;

      pixel_color += amb;
      // Set the pixel color
      setPixel(x + x_offset, y + y_offset, pixel_color(0), pixel_color(1), pixel_color(2));
    }
  }
  /*
  for (int i = 0 ; i < pl_pos.size(); i++) {
    Vector3d lightPos = pl_pos[i]*radius;
    for (int x = 0; x < 5; x++) {
      for (int y = 0; y < 5; y++) { 
        setPixel(lightPos(0)+trans_x+x, lightPos(1)+trans_y+y, pl_color[i](0), pl_color[i](1), pl_color[i](2));
      }
    }
  }
  */
  glEnd();
}

void shade_floor(int radius) {
  glBegin(GL_POINTS);
  Vector3d normal_hat = Vector3d(0.0,cos(0.25),-sin(0.25));
  for (int x = 0; x < viewport.w; x++) {
    for (int y = 0; y < viewport.h; y++) {  
      Vector3d pixel_color = Vector3d::Zero();
    
      Vector3d pos = Vector3d(x, 0, -y);

      // Calculate intensity
      Vector3d intensity = Vector3d::Zero();

      // Loop over point lights
      for( int i = 0; i < pl_color.size(); i++ ) {
        Vector3d i_pl = ((pl_pos[i] * radius) + Vector3d(trans_x, trans_y, 0) - pos);
        // Diffuse light
        Vector3d i_hat_pl = i_pl.normalized();
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
        Vector3d i_dl = dl_dir[i];
        // Diffuse light
        Vector3d i_hat_dl = -dl_dir[i].normalized();
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
//      cout << pixel_color(0) << ", " << pixel_color(1) << ", " << pixel_color(2) << endl;
      // Set the pixel color
      setPixel(x, y/2, pixel_color(0)*(viewport.h-y)/viewport.h, pixel_color(1)*(viewport.h-y)/viewport.h, pixel_color(2)*(viewport.h-y)/viewport.h);

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

 
  // This should be done before any other objects are shaded
  // so that other objects go on top of it
  if( gnd ) {
    shade_floor(min(viewport.w, viewport.h) / 2.5);
  }
  for( int i = 0; i < spheres.size(); i++ ) {
    shaded_sphere(spheres[i].radius, spheres[i].x_offset, spheres[i].y_offset);
  }
  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
  if( ss ) {
    char filename[256];
    sprintf(filename, "output.png");
    save_opengl_image(viewport.w, viewport.h, filename);
    exit(0);
  }
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
  cout << "Usage is -ka r g b -kd r g b -ks r g b -sp v -pl x y z r g b -dl x y z r g b -toon intervals -gnd -ss -ms\n";
  exit(0);
}

void processNormalKeys(unsigned char key, int x, int y) {
  if( key == 32 )
    exit(0);
  if ( key == 'x') {
    for (int i = 0; i < pl_pos.size(); i++) {
      cout << "-pl " << pl_pos[i].transpose() << " " << pl_color[i].transpose() << " ";
    }
    cout << endl; 
  }
#if BULLET
  if (key == '>') {

    vector<vector<double> > offsets;
    step_simulation(offsets);
    spheres.resize(0);
    for (int  j = 0; j < offsets.size(); j++) { 
      sphere_t sphere1 = {min(viewport.w, viewport.h) / 25, trans_x+offsets[j][0]*10, trans_y + offsets[j][1]*10 - 200};
      spheres.push_back(sphere1);
    }
    glutPostRedisplay();
    char filename[256];
    sprintf(filename, "images/nom-%.4d.png", fileout_count);
    fileout_count++;
    save_opengl_image(viewport.w, viewport.h, filename); 
  }
#endif



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
  }

  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = 600;
  viewport.h = 600;

  //spheres.clear();
  if(ms) {
    viewport.w = 1200;
    viewport.h = 600;
    trans_x = viewport.w/4.0;
    trans_y = viewport.h/2.0;
    sphere_t sphere2 = {min(viewport.w, viewport.h) / 3.0, trans_x + viewport.w/2.0, trans_y};
    sphere_t sphere3 = {min(viewport.w, viewport.h) / 3.0, trans_x, trans_y};
    spheres.push_back(sphere2);
    spheres.push_back(sphere3);
  }
  else {
    trans_x = viewport.w / 2.0;
    trans_y = viewport.h / 2.0;
    sphere_t sphere1 = {min(viewport.w, viewport.h) / 2.5, trans_x, trans_y};
    spheres.push_back(sphere1);
  }

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
  
#if BULLET
  initializeBulletWorld();
#endif

  glutMainLoop();							// infinite loop that will keep drawing and resizing and whatever else

  return 0;
}
