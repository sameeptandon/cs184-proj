#!gmake 

#Machine Architecture
MACHINE= $(shell uname -s)

#-----------------------------------------
# X       --------------------------------
X_INC  =   -I/usr/X11R6/include -I/sw/include -I/usr/sww/include -I/usr/sww/pkg/Mesa/include -I/usr/include/eigen2 -I./
X_LIB  =   -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib -L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -L/usr/local/lib -L./

#-----------------------------------------

#-----------------------------------------
# GL      --------------------------------
GL_LIB  =   -lglut -lGLU -lGL -lX11 -lfreeimage


#-----------------------------------------
#Basic Stuff -----------------------------
CC          = g++ -ggdb
cc          = gcc

#-----------------------------------------

#Optimization ----------------------------
OPT   = -O3 
#-----------------------------------------

ifeq ($(MACHINE),Darwin)
	LIBS = -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -L"/System/Library/Frameworks/GLUT.framework" -framework GLUT -framework OpenGL -lGL -lGLU -L/usr/X11R6/lib -L/opt/local/lib -lfreeimage 
	INCS = -I/usr/local/include/ -I/usr/include/ -I/opt/local/include/eigen2/ -I/opt/local/include/ 
	COMPILE_FLAG = -DOSX
else
	LIBS = $(X_LIB) $(GL_LIB)
	INCS = $(X_INC)
endif


CCOPTS = $(OPT) $(INCS) $(COMPILE_FLAG)
LDOPTS = $(OPT) $(INCS) $(LIBS) $(COMPILE_FLAG)

#-----------------------------------------

#-----------------------------------------

TARGETS = render

OBJECTS = render.o

default: $(TARGETS)

clean:
	/bin/rm -f *.o *.dSYM $(TARGETS)

#-----------------------------------------
#-----------------------------------------

render: Render.cpp Ray.o Sphere.o Camera.o RayTracer.o Scene.o PointLight.o DirectionalLight.o Triangle.o Ellipsoid.o Box.o Shape.o Node.o
	$(CC) Render.cpp Ray.o Sphere.o Camera.o RayTracer.o Scene.o PointLight.o DirectionalLight.o Triangle.o Ellipsoid.o Box.o Shape.o Node.o $(LDOPTS) -o render

PointLight.o: PointLight.cpp PointLight.h
	$(CC) PointLight.cpp -c $(CCOPTS)

DirectionalLight.o: DirectionalLight.cpp DirectionalLight.h
	$(CC) DirectionalLight.cpp -c $(CCOPTS)

Ray.o: Ray.cpp Ray.h
	$(CC) Ray.cpp -c $(CCOPTS)

Shape.o: Shape.h
	$(CC) Shape.cpp -c $(CCOPTS)

Triangle.o: Triangle.cpp Triangle.h Shape.h
	$(CC) Triangle.cpp -c $(CCOPTS)

Sphere.o: Sphere.cpp Sphere.h Shape.h
	$(CC) Sphere.cpp -c $(CCOPTS)

Ellipsoid.o: Ellipsoid.cpp Ellipsoid.h Sphere.h Shape.h
	$(CC) Ellipsoid.cpp -c $(CCOPTS)

Camera.o: Camera.cpp Camera.h
	$(CC) Camera.cpp -c $(CCOPTS)

RayTracer.o: RayTracer.cpp RayTracer.h
	$(CC) RayTracer.cpp -c $(CCOPTS)

Scene.o: Scene.cpp Scene.h
	$(CC) Scene.cpp -c $(CCOPTS)

Box.o: Box.cpp Box.h Ray.h
	$(CC) Box.cpp -c $(CCOPTS)

Node.o: Node.cpp Node.h Ray.h Globals.h Shape.h
	$(CC) Node.cpp -c $(CCOPTS)
#export LD_LIBRARY_PATH="/usr/sww/pkg/Mesa/lib:/usr/lib:/usr/sww/lib"

