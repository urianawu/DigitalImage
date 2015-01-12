// =============================================================================
// VIZA654/CSCE646 at Texas A&M University
// Homework 0
// Created by Anton Agana based from Ariel Chisholm's template
// 05.23.2011
//
// This file is supplied with an associated makefile. Put both files in the same
// directory, navigate to that directory from the Linux shell, and type 'make'.
// This will create a program called 'pr01' that you can run by entering
// 'homework0' as a command in the shell.
//
// If you are new to programming in Linux, there is an
// excellent introduction to makefile structure and the gcc compiler here:
//
// http://www.cs.txstate.edu/labs/tutorials/tut_docs/Linux_Prog_Environment.pdf
//
// =============================================================================
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <GLUT/glut.h>

#include <fstream>
#include <cassert>
#include <sstream>
#include <string>
#include <cmath>
#include <random>
#include <ctime>       /* time */
#include <vector>
#include <algorithm>

#include "ppmview.h"
#include "Vector2D.h"

#define PI 3.1415

using namespace std;

// =============================================================================
// These variables will store the input ppm image's width, height, and color
// =============================================================================

int inputwidth, inputheight, level, outputwidth, outputheight, outlevel;
unsigned char *in, *out;


GLint mainWindow, originalWindow;  //windows

// =============================================================================
// setPixels()
//
// This function stores the RGB values of each pixel to "pixmap."
// Then, "glutDisplayFunc" below will use pixmap to display the pixel colors.
// =============================================================================
void setPixels(char *infile)
{
    ppmview input;
    
    input.readPPM(infile);
    //input.readPGM(infile);
    inputwidth = input.width;
    inputheight = input.height;
    level = input.maxcolor;
    
    
    in = new unsigned char[inputwidth * inputheight * 3];
    in = input.pixmap;
    
}
void initializeOutput()
{
    outputwidth = inputwidth;
    outputheight = inputheight;
    out = new unsigned char[outputwidth * outputheight * 3];
    for (int i = 0; i < outputwidth * outputheight * 3; i++) {
        out[i] = in[i];
    }
}
int find_closest_palette_color(int oldpixel)
{
    int newpixel;
    int step = level / outlevel;
    newpixel = round(oldpixel*1.0/255*outlevel) * step;
    return newpixel;
}


/**
 -------Algorithm--------
 for each y from top to bottom
 for each x from left to right
 oldpixel  := pixel[x][y]
 newpixel  := find_closest_palette_color(oldpixel)
 pixel[x][y]  := newpixel
 quant_error  := oldpixel - newpixel
 pixel[x+1][y  ] := pixel[x+1][y  ] + 7/16 * quant_error
 pixel[x-1][y+1] := pixel[x-1][y+1] + 3/16 * quant_error
 pixel[x  ][y+1] := pixel[x  ][y+1] + 5/16 * quant_error
 pixel[x+1][y+1] := pixel[x+1][y+1] + 1/16 * quant_error
 */
void FSErrorDiffusion()
{
    
    for (int y = outputheight-1; y >= 0; y--) {
        for (int x = 0; x < outputwidth; x++) {
            int pixel = (y * outputwidth + x) * 3;
            for (int i = 0; i < 3; i++) {
                
                
                int oldpixel = out[pixel + i];
                int newpixel = find_closest_palette_color(oldpixel);
                out[pixel + i] = newpixel;
                int quant_error = oldpixel - newpixel;
                int temp = 0;
                
                if (x < outputwidth - 1) {
                    temp = out[(y * outputwidth + x + 1) * 3 + i] + 7/16 * (float)quant_error;
                    if (temp >= 0 && temp <= 255) {
                        out[(y * outputwidth + x + 1) * 3 + i] = temp;
                    }else if (temp < 0 ) {
                        out[(y * outputwidth + x + 1) * 3 + i] = 0;
                    }else {
                        out[(y * outputwidth + x + 1) * 3 + i] = 255;
                    }
                }
                
                if (y > 1 && x > 1) {
                    
                    temp = out[((y - 1) * outputwidth + x - 1) * 3 + i] + 3/16 * (float)quant_error;
                    if (temp >= 0 && temp <= 255) {
                        out[((y - 1) * outputwidth + x - 1) * 3 + i] = temp;
                    }else if (temp < 0 ) {
                        out[((y - 1) * outputwidth + x - 1) * 3 + i] = 0;
                    }else {
                        out[((y - 1) * outputwidth + x - 1) * 3 + i] = 255;
                    }
                }
                
                if (y > 1) {
                    
                    temp = out[((y - 1) * outputwidth + x) * 3 + i] + 5/16 * (float)quant_error;
                    if (temp >= 0 && temp <= 255) {
                        out[((y - 1) * outputwidth + x) * 3 + i] = temp;
                    }else if (temp < 0 ) {
                        out[((y - 1) * outputwidth + x) * 3 + i] = 0;
                    }else {
                        out[((y - 1) * outputwidth + x) * 3 + i] = 255;
                    }
                }
                
                if (y > 1 && x < outputwidth - 1) {
                    
                    temp = out[((y - 1) * outputwidth + x + 1) * 3 + i] + 1/16 * (float)quant_error;
                    if (temp >= 0 && temp <= 255) {
                        out[((y - 1) * outputwidth + x + 1) * 3 + i] = temp;
                    }else if (temp < 0 ) {
                        out[((y - 1) * outputwidth + x + 1) * 3 + i] = 0;
                    }else {
                        out[((y - 1) * outputwidth + x + 1) * 3 + i] = 255;
                    }
                }
            }
        }
    }
 
    
}


/**
 ------Algorithm------
 for each y
 for each x
 oldpixel := pixel[x][y] + threshold_map_4x4[x mod 4][y mod 4]
 newpixel := find_closest_palette_color(oldpixel)
 pixel[x][y] := newpixel
 */
void OrderedDither()
{
    int threshold_map[8][8] = {{1,49,13,61,4,52,16,64},{33,17,45,29,36,20,48,32},{9,57,5,53,12,60,8,56},{41,25,37,21,44,28,40,24},
                               {3,51,15,63,2,50,14,62},{35,19,47,31,34,18,46,30},{11,59,7,55,10,58,6,54},{43,27,39,23,42,26,38,22}};
    
    //int threshold_map4[4][4] = {{1,9,3,11},{13,5,15,7},{4,12,2,10},{16,8,14,6}};
    for (int y = 0; y < outputheight; y++) {
        for (int x = 0; x < outputwidth; x++) {
            int pixel = (y * outputwidth + x) * 3;
            for (int i = 0; i < 3; i++) {
                
            
            int oldpixel = out[pixel+i] + (1.0/65 * (float)threshold_map[x % 8][y % 8]) * level/outlevel;
            if (oldpixel < 0 ) {
                oldpixel = 0;
            }
            if (oldpixel > 255) {
                oldpixel = 255;
            }
            
            int newpixel = find_closest_palette_color(oldpixel);
            out[pixel+i] = newpixel;
            }
        }
    }
}

void RandNoiseDither()
{
    for (int y = 0; y < outputheight; y++) {
        for (int x = 0; x < outputwidth; x++) {
            int pixel = (y * outputwidth + x) * 3;
            float randNum = (1.0*rand() /RAND_MAX );
            for (int i = 0; i < 3; i++) {
                float oldpixel = out[pixel + i] + ( randNum - 0.5) * round(level * 1.0 / outlevel);
                if (oldpixel < 0 ) {
                    oldpixel = 0;
                }
                
                if (oldpixel > 255) {
                    oldpixel = 255;
                }
                int newpixel = find_closest_palette_color(oldpixel);
                out[pixel+i] = newpixel;

            }
        }
    }

}

void Screening(int size)
{
    for (int y = outputheight-1; y >= 0; y-=size) {
        for (int x = 0; x < outputwidth; x+=size) {
            int pixel = (y * outputwidth + x) * 3;
            int total_color = 0;
			int count = 0;
			for(int i = 0; i < size; i++){
				for(int j = 0; j < size; j++){
					pixel=((y-i)*outputwidth+x+j)*3;
					if( y-i >= 0 && x+j < outputwidth){
						total_color += out[pixel];
						total_color += out[pixel + 1];
						total_color += out[pixel + 2];
						count = count + 3;
					}
				}
			}
			total_color = total_color/count;
            float r = size*sqrt(total_color/255.0 /3.14);
			int center_y=y-size/2.0;
			int center_x=x+size/2.0;
			for(int i=0;i<size;i++){
				for(int j=0;j<size;j++){
					pixel=((y-i)*outputwidth+x+j)*3;
					if( y-i >0 && x+j <outputwidth){
						float temp_x= x + j;
						float temp_y= y - i;
						float dis= sqrt( pow(temp_x-center_x,2)+pow(temp_y-center_y,2) );
						if(dis > r){
							out[pixel] = 0;
							out[pixel+1] = 0;
							out[pixel+2] = 0;
						}
						else{
							out[pixel] = 255;
							out[pixel+1] = 255;
							out[pixel+2] = 255;
						}
					}
				}
			}
            
        }
    }
}

void outputImage(char *outfile)
{
    ppmview output;
    output.width = outputwidth;
    output.height = outputheight;
    output.maxcolor = outlevel;
    output.pixmap = out;
    if (outfile != NULL) {
        output.writePPM(outfile,0);
        //output.writePGM(outfile, 0);
    }

}

// =============================================================================
// OpenGL Display and Mouse Processing Functions.
//
// You can read up on OpenGL and modify these functions, as well as the commands
// in main(), to perform more sophisticated display or GUI behavior. This code
// will service the bare minimum display needs for most assignments.
// =============================================================================
static void OwindowResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,(w/2),0,(h/2),0,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
static void OwindowDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1,1,1,1); // Set background color.
    glRasterPos2i(0,0);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glDrawPixels(inputwidth, inputheight, GL_RGB, GL_UNSIGNED_BYTE, in);
    glutSwapBuffers();
}

static void windowResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,(w/2),0,(h/2),0,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
static void windowDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1,1,1,1); // Set background color.
    
    glRasterPos2i(0,0);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glDrawPixels(outputwidth, outputheight, GL_RGB, GL_UNSIGNED_BYTE, out);
    
    glutSwapBuffers();
}

static void processMouse(int button, int state, int x, int y)
{
    
}


static void init(void)
{
    glClearColor(1,1,1,1); // Set background color.
   
}

void idle()
{
    //glutSetWindow(mainWindow);
    //glutSetWindow(originalWindow);
    glutPostRedisplay();

}

// =============================================================================
// main() Program Entry
// =============================================================================
int main(int argc, char *argv[])
{
    char * filename = {};
    char *outFile = {};
    char operation[10];
    int size;
    
    if (argc == 1) {
        printf("Error! No input file.\n");
    }else if (argc == 2) {
        filename = argv[1];
    }else if (argc == 3) {
        filename = argv[1];
        outFile = argv[2];
    }else {
        printf("Error! Too many arguments.\n");
    }
    
    setPixels(filename);
    
    //reduce color
    printf("The original image has %d levels. Reduce it to ?\n"
           "Please choose from 2, 4, 8, 16, 32, 64, 128:", level);
    scanf("%d",&outlevel);
    while (outlevel != 2 && outlevel != 4 && outlevel != 8 && outlevel != 16 && outlevel != 32 && outlevel != 64 && outlevel != 128 ) {
        printf("Invalid input, please try again.\n");
        printf("The original image has %d levels. Reduce it to ?\n"
               "Please choose from 2, 4, 8, 16, 32, 64, 128 ", level);
        scanf("%d",&outlevel);
    }
    //operation control
    printf("Enter the operation you want to perform:\n"
           "f  - Floyd-Steinberg Error diffusion\n"
           "o  - Ordered Dither\n"
           "r  - Random Noise Dither\n"
           "a  - Artistic Screening\n");
    
    printf("Enter operation:");
    scanf("%s", operation);
    while (operation[0] != 'f' && operation[0] != 'o' && operation[0] != 'r' && operation[0] != 'a') {
        printf("Invalid input, please try again.\n");
        printf("Enter operation:");
        scanf("%s", operation);
    }
    
    initializeOutput();
    if (operation[0] == 'f') {
        FSErrorDiffusion();
    }
    if (operation[0] == 'o') {
        OrderedDither();
    }
    if (operation[0] == 'r') {
        RandNoiseDither();
    }
    if (operation[0] == 'a') {
        printf("Enter screening size:");
        scanf("%d",&size);
        Screening(size);
    }
    
    if (argc == 3) {
        outputImage(outFile);
        printf("[Your program will display and save the output image.]\n");
    }

    // OpenGL Commands:
    // Once "glutMainLoop" is executed, the program loops indefinitely to all
    // glut functions.
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100); // Where the window will display on-screen.
    glutInitWindowSize(inputwidth, inputheight);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    originalWindow = glutCreateWindow("Input Image");
    init();
    glutReshapeFunc(OwindowResize);
    glutDisplayFunc(OwindowDisplay);
    
    //output image display
    glutInitWindowPosition(100+inputwidth, 100); // Where the window will display on-screen.
    
    glutInitWindowSize(outputwidth, outputheight);
    
    mainWindow = glutCreateWindow("Output Image");
    glutReshapeFunc(windowResize);
    glutDisplayFunc(windowDisplay);
    
    glutMouseFunc(processMouse);
    glutIdleFunc(idle);

    glutMainLoop();

    
    return 0; //This line never gets reached. We use it because "main" is type int.
}

