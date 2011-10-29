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

TARGETS = bezier

OBJECTS = bezier.o

default: $(TARGETS)

clean:
	/bin/rm -f *.o *.dSYM $(TARGETS)

#-----------------------------------------
#-----------------------------------------

bezier: Bezier.cpp BezierPatch.o Globals.h Triangle.o
	$(CC) Bezier.cpp BezierPatch.o Triangle.o $(LDOPTS) -o bezier

BezierPatch.o: BezierPatch.cpp Globals.h
	$(CC) BezierPatch.cpp -c $(CCOPTS)

Triangle.o: Triangle.cpp Triangle.h Globals.h
	$(CC) Triangle.cpp -c $(CCOPTS)
#export LD_LIBRARY_PATH="/usr/sww/pkg/Mesa/lib:/usr/lib:/usr/sww/lib"

