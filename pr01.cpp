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
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <fstream>
#include <cassert>
#include <sstream>
#include <string>
#include <cmath>

#define pi 3.14159265358

using namespace std;

// =============================================================================
// These variables will store the input ppm image's width, height, and color
// =============================================================================
int width, height;
unsigned char *pixmap;

double theta(double a){
    if (a<0) {
        return 0;
    }else
        return 1;
    
}
int sgn(double a){
    if (a<0) {
        return -1;
    }else if(a==0){
        return 0;
    }else
        return 1;
}
// =============================================================================
// setPixels()
//
// This function stores the RGB values of each*pixel to "pixmap."
// Then, "glutDisplayFunc" below will use pixmap to display the pixel colors.
// =============================================================================
void setPixels(int r, int g, int b, int special)
{
    
    if (special == 1) {
        for(int y = 0; y < height/2 ; y++) {
            for(int x = 0; x < width/2; x++) {
                int i = (y * width + x) * 3;
                pixmap[i++] = 0x00;
                pixmap[i++] = 0x00; //Do you know what "0xFF" represents? Google it!
                pixmap[i] = b; //Learn to use the "0x" notation to your advantage.
            }
        }
        for(int y = height/2; y < height ; y++) {
            for(int x = 0; x < width/2; x++) {
                int i = (y * width + x) * 3;
                pixmap[i++] = r;
                pixmap[i++] = 0x00; //Do you know what "0xFF" represents? Google it!
                pixmap[i] = 0x00; //Learn to use the "0x" notation to your advantage.
            }
        }
        for(int y = 0; y < height/2 ; y++) {
            for(int x = width/2; x < width; x++) {
                int i = (y * width + x) * 3;
                pixmap[i++] = r;
                pixmap[i++] = g; //Do you know what "0xFF" represents? Google it!
                pixmap[i] = 0x00; //Learn to use the "0x" notation to your advantage.
            }
        }
        for(int y = height/2; y < height ; y++) {
            for(int x = width/2; x < width; x++) {
                int i = (y * width + x) * 3;
                pixmap[i++] = 0x00;
                pixmap[i++] = g; //Do you know what "0xFF" represents? Google it!
                pixmap[i] = 0x00; //Learn to use the "0x" notation to your advantage.
            }
        }
    }else if (special == 2){
        for(int y = 0; y < height ; y++) {
            for(int x = 0; x < width; x++) {
                int i = (y * width + x) * 3;
                if (((x-width/2)*(x-width/2) + (y-height/2)*(y-height/2)) <= (height/4)*(height/4) ) {
                    pixmap[i++] = r;
                    pixmap[i++] = g; //Do you know what "0xFF" represents? Google it!
                    pixmap[i] = 0x00;
                }else{
                    pixmap[i++] = 0x00;
                    pixmap[i++] = 0x00; //Do you know what "0xFF" represents? Google it!
                    pixmap[i] = b;
                }
            }
        }
    }else if (special == 3){
        //BACKGROUND
        for (int y = 0; y < height ; y++) {
            for(int x = 0; x < width; x++) {
                int i = (y * width + x) * 3;
                //CIRCLE
                if (((x-width/2)*(x-width/2) + (y-height/2-height/8)*(y-height/2-height/8)) <= (height/8)*(height/8) ) {
                    pixmap[i++] = r;
                    pixmap[i++] = g; 
                    pixmap[i] = 0x00;
                    //torso
                }else if (((x-width/2) < height/8
                           && (x-width/2) > -height/8) && ((y-height/2) < height/8
                                                          && (y-height/2) > -height/16) ){
                    pixmap[i++] = r;
                    pixmap[i++] = g; 
                    pixmap[i] = 0x00;
                    //CLOTH
                }else if(((x-width/2) < height/8
                          && (x-width/2) > -height/8) && ((y-height/2) <= -height/16
                                                          && (y-height/2) > -height/4)){
                    pixmap[i++] = 0x00;
                    pixmap[i++] = 0x00;
                    pixmap[i] = b;
                    //Hands
                }else if (((x-width/2) < -height/8
                           && (x-width/2) > -height/7) && ((y-height/2) > -height/4
                                                           && (y-height/2) < -height/8) ){
                    pixmap[i++] = r;
                    pixmap[i++] = g;
                    pixmap[i] = 0x00;
                }else if (((x-width/2) > height/8
                           && (x-width/2) < height/7) && ((y-height/2) > -height/4
                                                           && (y-height/2) < -height/8) ){
                    pixmap[i++] = r;
                    pixmap[i++] = g;
                    pixmap[i] = 0x00;
                    //gloves
                }else if (((x-width/2) < -height/8
                           && (x-width/2) > -height/7) && ((y-height/2) < -height/4
                                                           && (y-height/2) > -height/3.7) ){
                    pixmap[i++] = 0x00;
                    pixmap[i++] = 0x00;
                    pixmap[i] = 0x00;
                }else if (((x-width/2) > height/8
                           && (x-width/2) < height/7) && ((y-height/2) < -height/4
                                                           && (y-height/2) > -height/3.7) ){
                    pixmap[i++] = 0x00;
                    pixmap[i++] = 0x00;
                    pixmap[i] = 0x00;
                    //legs
                }else if(((x-width/2) > height/32
                          && (x-width/2) < height/12) && ((y-height/2) <= -height/4
                                                          && (y-height/2) > -height/3.5)){
                    pixmap[i++] = 0x00;
                    pixmap[i++] = 0x00;
                    pixmap[i] = b;
                    
                }else if(((x-width/2) < -height/32
                          && (x-width/2) > -height/12) && ((y-height/2) <= -height/4
                                                           && (y-height/2) > -height/3.5)){
                    pixmap[i++] = 0x00;
                    pixmap[i++] = 0x00;
                    pixmap[i] = b;
                    //shoes
                }else if(((x-width/2) > height/32
                          && (x-width/2) < height/12) && ((y-height/2) <= -height/3.5
                                                          && (y-height/2) > -height/3.3)){
                    pixmap[i++] = 0x00;
                    pixmap[i++] = 0x00;
                    pixmap[i] = 0x00;
                    
                }else if(((x-width/2) < -height/31.8
                          && (x-width/2) > -height/12.2) && ((y-height/2) <= -height/3.5
                                                           && (y-height/2) > -height/3.3)){
                    pixmap[i++] = 0x00;
                    pixmap[i++] = 0x00;
                    pixmap[i] = 0x00;
                }else{
                pixmap[i++] = 0xFF;
                pixmap[i++] = 0xFF; 
                pixmap[i] = 0xFF;
            }
            
        }
        }
        //DETAILS
        for (int y = 0; y < height ; y++) {
            for(int x = 0; x < width; x++) {
                int i = (y * width + x) * 3;
                //cutouts
                if (((x-width/2) < -height/16
                     && (x-width/2) > -height/8) && ((y-height/2) <= -height/10
                                                     && (y-height/2) > -height/6)) {
                    pixmap[i++] = r;
                    pixmap[i++] = g;
                    pixmap[i] = 0x00;
                }else if (((x-width/2) > height/16
                           && (x-width/2) < height/8) && ((y-height/2) <= -height/10
                                                           && (y-height/2) > -height/6)){
                    pixmap[i++] = r;
                    pixmap[i++] = g;
                    pixmap[i] = 0x00;
                    //eye
                }else if(((x-width/2)*(x-width/2) + (y-height/2-height/8)*(y-height/2-height/8)) <= (height/100)*(height/100) ) {
                    pixmap[i++] = 0x04;
                    pixmap[i++] = 0x03;
                    pixmap[i] = 0x01;
                }else if(((x-width/2)*(x-width/2) + (y-height/2-height/8)*(y-height/2-height/8)) <= (height/40)*(height/40) ) {
                    pixmap[i++] = 0x44;
                    pixmap[i++] = 0x33;
                    pixmap[i] = 0x41;
                }else if(((x-width/2)*(x-width/2) + (y-height/2-height/8)*(y-height/2-height/8)) <= (height/16)*(height/16) ) {
                    pixmap[i++] = 0xEF;
                    pixmap[i++] = 0xEF;
                    pixmap[i] = 0xEF;
                    //glass
                }else if(((x-width/2)*(x-width/2) + (y-height/2-height/8)*(y-height/2-height/8)) <= (height/13)*(height/13) ) {
                    pixmap[i++] = 0xDF;
                    pixmap[i++] = 0xDF;
                    pixmap[i] = 0xDF;
                }else if(((x-width/2) < -height/32
                          && (x-width/2) > -height/8) && ((y-height/2) >= height/9
                                                           && (y-height/2) < height/7)){
                    pixmap[i++] = 0x00;
                    pixmap[i++] = 0x00;
                    pixmap[i] = 0x00;
                }else if(((x-width/2) > height/32
                          && (x-width/2) < height/8) && ((y-height/2) >= height/9
                                                          && (y-height/2) < height/7)){
                    pixmap[i++] = 0x00;
                    pixmap[i++] = 0x00;
                    pixmap[i] = 0x00;
                }
            }
        }
        
        
        //SQUARE-BODY
        //
        
        
    }else{
        for(int y = 0; y < height ; y++) {
            for(int x = 0; x < width; x++) {
                int i = (y * width + x) * 3;
                pixmap[i++] = r;
                pixmap[i++] = g; //Do you know what "0xFF" represents? Google it!
                pixmap[i] = b; //Learn to use the "0x" notation to your advantage.
            }
        }
    }
}


void draw(char * select)
{
    switch (select[0]) {
        case 'r':
            setPixels(0xFF,0x00,0x00,0);
            break;
        case 'g':
            setPixels(0x00,0xFF,0x00,0);
            break;
        case 'b':
            setPixels(0x00,0x00,0xFF,0);
            break;
        case 'a':
            setPixels(0XFF,0xFF,0xFF,1);
            break;
        case 'c':
            setPixels(0xFF,0xFF,0xFF,2);
            break;
        case 's':
            setPixels(0xFF,0xFF,0xFF,3);
            break;
        default:
            break;
    }
}


// =============================================================================
// OpenGL Display and Mouse Processing Functions.
//
// You can read up on OpenGL and modify these functions, as well as the commands
// in main(), to perform more sophisticated display or GUI behavior. This code
// will service the bare minimum display needs for most assignments.
// =============================================================================
static void windowResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,(w/2),0,(h/2),0,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
static void windowDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(0,0);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixmap);
    glFlush();
}
static void processMouse(int button, int state, int x, int y)
{
    if(state == GLUT_UP)
        exit(0);               // Exit on mouse click.
}
static void init(void)
{
    glClearColor(1,1,1,1); // Set background color.
}

// =============================================================================
// main() Program Entry
// =============================================================================
int main(int argc, char *argv[])
{
    
    char select[] = "";
    int input;
    //initialize the global variables
    width = 640;
    height = 480;
    pixmap = new unsigned char[width * height * 3];  //Do you know why "3" is used?

    input = argc;
    if (input != 2)
        strcpy(select,"");
    else
        strcpy(select,argv[1]);

    while (input != 2 || (strcmp(select, "red") != 0 && strcmp(select, "green") != 0
                          && strcmp(select, "blue") != 0 && strcmp(select, "all") != 0
                          && strcmp(select, "circle") != 0 && strcmp(select, "special") != 0)) {
        printf("ERROR: Invalid input.\nPlease choose from: red/green/blue/all/circle/special\n");
        gets(select);
        if(strstr(select," ") == 0) 
            input = 2;
        else
            input = 0;
    }
    
    draw(select);
    //setPixels();
    
    
    // OpenGL Commands:
    // Once "glutMainLoop" is executed, the program loops indefinitely to all
    // glut functions.
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100); // Where the window will display on-screen.
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Project 1");
    init();
    glutReshapeFunc(windowResize);
    glutDisplayFunc(windowDisplay);
    glutMouseFunc(processMouse);
    glutMainLoop();
    
    return 0; //This line never gets reached. We use it because "main" is type int.
}

