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
CC          = g++ 
cc          = gcc

#-----------------------------------------

#Optimization ----------------------------
OPT   = -O3 
#-----------------------------------------

ifeq ($(MACHINE),Darwin)
	LIBS = -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -L"/System/Library/Frameworks/GLUT.framework" -framework GLUT -framework OpenGL -lGL -lGLU -L/usr/X11R6/lib
	INCS = -I/usr/local/include/ -I/usr/include/ -I/opt/local/include/eigen2/ -I/opt/local/include/ 
else
	LIBS = $(X_LIB) $(GL_LIB)
	INCS = $(X_INC)
endif


CCOPTS = $(OPT) $(INCS)
LDOPTS = $(OPT) $(INCS) $(LIBS)

#-----------------------------------------

#-----------------------------------------

TARGETS = raytrace

OBJECTS = raytrace.o

default: $(TARGETS)

clean:
	/bin/rm -f *.o $(TARGETS)

#-----------------------------------------
#-----------------------------------------

raytrace: raytrace.cpp ray.o
	$(CC) raytrace.cpp ray.o $(LDOPTS) -o raytrace 

ray.o: ray.cpp ray.h
	$(CC) ray.cpp -c $(CCOPTS)

ray.o: sphere.cpp sphere.h shape.h
	$(CC) sphere.cpp -c $(CCOPTS)

#export LD_LIBRARY_PATH="/usr/sww/pkg/Mesa/lib:/usr/lib:/usr/sww/lib"

