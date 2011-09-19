// Simple OpenGL example for CS184 F06 by Nuttapong Chentanez, modified from sample code for CS184 on Sp06
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "algebra3.h"

#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
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
vec3 ka = vec3(255.0f/255.0f, 215.0f/255.0f, 0.0f/255.0f);
vec3 kd = vec3(255.0f/255.0f, 69.0f/255.0f, 0.0f/255.0f);
vec3 ks = vec3(255.0f/255.0f, 255.0f/255.0f, 224.0f/255.0f);
// Default lighting
vec3 pl_pos = vec3();
vec3 pl_color = vec3();
vec3 dl_pos = vec3();
vec3 dl_color = vec3();
// Default specular term power
float sp = .5f;

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

void circle(int x, int y, int radius) {
	// Draw inner circle
	glBegin(GL_POINTS);

	for (int i = -radius; i <= radius; i++) {
		int width = (int)(sqrt((float)(radius*radius-i*i)) + 0.5f);
		for (int j = -width; j <= width; j++) {

			// Set the red pixel
			setPixel(x + j, y + i, 1.0f, 0.0f, 0.0f);
		}
	}
	glEnd();

	// Draw border, using line strip primitive
	glBegin(GL_LINE_STRIP);
	
	// White border
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = 0; i <= 360; i++)
	{
		// Need to perform similar coordinate conversion
		float radian = i*PI/180.0f;
		glVertex2f(x + cos(radian)*radius, y + sin(radian)*radius);
	}

	glEnd();
}
//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {
	
	glClear(GL_COLOR_BUFFER_BIT);				// clear the color buffer
	
	glMatrixMode(GL_MODELVIEW);					// indicate we are specifying camera transformations
	glLoadIdentity();							// make sure transformation is "zero'd"


	// Start drawing
	//circle(plotX, plotY, min(viewport.w, viewport.h) / 4);

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

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {

	// Read command line arguments
	int i = 0;
	if(i + 1 != argc) {
		if (argv[i] == "-ka" && i + 3 < argc) {
			ka = vec3(atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3]));
		}
		else if (argv[i] == "-kd" && i + 3 < argc) {
			kd = vec3(atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3]));
		}
		else if (argv[i] == "-ks" && i + 3 < argc) {
			ks = vec3(atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3]));
		}
		else if (argv[i] == "-sp" && i + 1 < argc) {
			sp = atof(argv[i+1]);
		}
		else if (argv[i] == "-pl" && i + 6 < argc) {
			pl_pos = vec3(atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3]));
			pl_color = vec3(atof(argv[i+4]), atof(argv[i+5]), atof(argv[i+6]));
		}
		else if (argv[i] == "-dl" && i + 6 < argc) {
			dl_pos = vec3(atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3]));
			dl_color = vec3(atof(argv[i+4]), atof(argv[i+5]), atof(argv[i+6]));
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
  	viewport.w = 400;
  	viewport.h = 400;

  	//The size and position of the window
  	glutInitWindowSize(viewport.w, viewport.h);
  	glutInitWindowPosition(0,0);
  	glutCreateWindow(argv[0]);

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








