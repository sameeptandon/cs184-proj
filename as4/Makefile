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
	LIBS = -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -L"/System/Library/Frameworks/GLUT.framework" -framework GLUT -framework OpenGL -lGL -lGLU -L/usr/X11R6/lib
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
	/bin/rm -f *.o $(TARGETS)

#-----------------------------------------
#-----------------------------------------

render: Render.cpp Ray.o Sphere.o Camera.o RayTracer.o Scene.o
	$(CC) Render.cpp Ray.o Sphere.o Camera.o RayTracer.o Scene.o $(LDOPTS) -o render

Ray.o: Ray.cpp Ray.h
	$(CC) Ray.cpp -c $(CCOPTS)

Sphere.o: Sphere.cpp Sphere.h Shape.h
	$(CC) Sphere.cpp -c $(CCOPTS)

Camera.o: Camera.cpp Camera.h
	$(CC) Camera.cpp -c $(CCOPTS)

RayTracer.o: RayTracer.cpp RayTracer.h
	$(CC) RayTracer.cpp -c $(CCOPTS)

Scene.o: Scene.cpp Scene.h
	$(CC) Scene.cpp -c $(CCOPTS)
#export LD_LIBRARY_PATH="/usr/sww/pkg/Mesa/lib:/usr/lib:/usr/sww/lib"

