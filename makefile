# Please see the header information in the supplied ppmview.ccp file for more
# information on this template project. For more information on C programming
# in Linux, please see the excellent introduction to makefile structure and
# the gcc compiler here:
#
# http://www.cs.txstate.edu/labs/tutorials/tut_docs/Linux_Prog_Environment.pdf

#This makefile runs on MacOS 10.8.5
# Put both files in the same directory, 
# navigate to that directory from the Terminal, 
# and type 'make'. 
#This will create a program called 'pr4'.
#NOTE: This project uses GLUI framework in Mac.
#GLUI can be downloaded here: http://lukecyca.com/2008/glui-235-framework-for-mac-os-x.html
#framework must be added to XCode framework path.
#generally, the path is: 
#/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/System/Library/Frameworks/GLUI.framework

CC		= g++
LDFLAGS 	= -framework openGL -framework GLUT -framework GLUI -lm
PROJECT		= pr4
FILES		= pr4.cpp ppmview.cpp ppmview.h Vector2D.h Vector2D.cpp colorSpaceConversion.h colorSpaceConversion.cpp

${PROJECT}:${FILES}
	${CC} ${CFLAGS} -o ${PROJECT} ${FILES} ${LDFLAGS}
