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
#This will create a program called 'ppmview'.

CC		= g++
LDFLAGS 	= -framework openGL -framework GLUT -lm
PROJECT		= ppmview
FILES		= ppmview.cpp

${PROJECT}:${FILES}
	${CC} ${CFLAGS} -o ${PROJECT} ${FILES} ${LDFLAGS}
