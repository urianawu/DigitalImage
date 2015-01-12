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
#include <cstring>
#include <cmath>
#include <algorithm>    // std::max
#include "Vector2D.h"

#define pi 3.14159265358

using namespace std;

// =============================================================================
// These variables will store the input ppm image's width, height, and color
// =============================================================================
int width, height;
unsigned char *pixmap;
int Num_Samples;

double theta(double a){
    if (a<0) {
        return 0;
    }else
        return 1;
    
}
int sgn(double a){
    if (a<0) {
        return -1;
    }else if(a=0){
        return 0;
    }else
        return 1;
}

//vertices, lines, and normals
Vector2D p0(300, 370);
Vector2D p1(190, 260);
Vector2D p2(230, 170);
Vector2D p3(404, 180);
Vector2D p4(390, 330);

Vector2D findNormals(Vector2D &v1, Vector2D &v2)
{
    Vector2D v12 = v1-v2;

    v12.Rotate(-90);
    v12.Normalize();
    return v12;
}

// =============================================================================
// setPixels()
//
// This function stores the RGB values of each*pixel to "pixmap."
// Then, "glutDisplayFunc" below will use pixmap to display the pixel colors.
// =============================================================================
void setConvex(float* cI, float* cO)
{
    
    Vector2D p01 = findNormals(p1,p0);
    Vector2D p12 = findNormals(p2,p1);
    Vector2D p23 = findNormals(p3,p2);
    Vector2D p34 = findNormals(p4,p3);
    Vector2D p40 = findNormals(p0,p4);
    

    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int i = (y * width + x) * 3;
            
            float color[3] = {0.0, 0.0, 0.0};
            float rx=rand()/RAND_MAX;
            float ry=rand()/RAND_MAX;
            for(float i=0;i<Num_Samples;i++){
                for(float j=0;j<Num_Samples;j++){
                    float x_sample=(rx+i)/Num_Samples;
                    float y_sample=(ry+j)/Num_Samples;
                    Vector2D p(x+x_sample,y+y_sample);
                    
                    
                    if (p01.DotProduct(p-p0) <= 0 && p12.DotProduct(p-p1) <= 0 && p23.DotProduct(p-p2) <= 0 && p34.DotProduct(p-p3) <= 0
                        && p40.DotProduct(p-p4) <= 0) {
                        
                        color[0] += cI[0];
                        color[1] += cI[1];
                        color[2] += cI[2];
                    }else {
                        color[0] += cO[0];
                        color[1] += cO[1];
                        color[2] += cO[2];
                    }
                }
            }
            pixmap[i++] = color[0] / powf(Num_Samples,2);
            pixmap[i++] = color[1] / powf(Num_Samples,2);
            pixmap[i] = color[2] / powf(Num_Samples,2);
        }
    }
}

void setStar(float* cI, float* cO)
{
    Vector2D p02 = findNormals(p2,p0);
    Vector2D p13 = findNormals(p3,p1);
    Vector2D p24 = findNormals(p4,p2);
    Vector2D p30 = findNormals(p0,p3);
    Vector2D p41 = findNormals(p1,p4);
    
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int i = (y * width + x) * 3;
            
            float color[3] = {0.0, 0.0, 0.0};
            float rx=rand()/RAND_MAX;
            float ry=rand()/RAND_MAX;
            for(float i=0;i<Num_Samples;i++){
                for(float j=0;j<Num_Samples;j++){
                    float x_sample=(rx+i)/Num_Samples;
                    float y_sample=(ry+j)/Num_Samples;
                    int flag = 0;
                    Vector2D p(x+x_sample,y+y_sample);
                    
                    if (p02.DotProduct(p-p0) <= 0)
                        flag +=1;
                    if (p13.DotProduct(p-p1) <= 0)
                        flag +=1;
                    if (p24.DotProduct(p-p2) <= 0)
                        flag +=1;
                    if (p30.DotProduct(p-p3) <= 0)
                        flag +=1;
                    if (p41.DotProduct(p-p4) <= 0)
                        flag +=1;
                    
                    if (flag >= 4) {
                        color[0] += cI[0];
                        color[1] += cI[1];
                        color[2] += cI[2];
                    }else {
                        color[0] += cO[0];
                        color[1] += cO[1];
                        color[2] += cO[2];
                    }
                }
            }
            pixmap[i++] = color[0] / powf(Num_Samples,2);
            pixmap[i++] = color[1] / powf(Num_Samples,2);
            pixmap[i] = color[2] / powf(Num_Samples,2);
            
        }
    }
}

void setFunction(float* cI, float* cO)
{
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int i = (y * width + x) * 3;
            float color[3] = {0.0, 0.0, 0.0};
            float rx=rand()/RAND_MAX;
            float ry=rand()/RAND_MAX;
            for(float i=0;i<Num_Samples;i++){
                for(float j=0;j<Num_Samples;j++){
                    float x_sample=(rx+i)/Num_Samples;
                    float y_sample=(ry+j)/Num_Samples;
                    Vector2D p(x+x_sample,y+y_sample);
                    
                    if ((p.y-height/2) - exp(-(float)p.x/(width/6))*height/4*sinf(2*pi*p.x/(width/6)) < 0) {
                        color[0] += cI[0];
                        color[1] += cI[1];
                        color[2] += cI[2];
                    }else {
                        color[0] += cO[0];
                        color[1] += cO[1];
                        color[2] += cO[2];
                    }
                }
            }
            pixmap[i++] = color[0] / powf(Num_Samples,2);
            pixmap[i++] = color[1] / powf(Num_Samples,2);
            pixmap[i] = color[2] / powf(Num_Samples,2);
        }
    }
}

void setBlobby(float* cI, float* cO)
{
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int i = (y * width + x) * 3;
            float color[3] = {0.0, 0.0, 0.0};
            float rx=rand()/RAND_MAX;
            float ry=rand()/RAND_MAX;
            for(float i=0;i<Num_Samples;i++){
                for(float j=0;j<Num_Samples;j++){
                    float x_sample=(rx+i)/Num_Samples;
                    float y_sample=(ry+j)/Num_Samples;
                    Vector2D p(x+x_sample,y+y_sample);
                    
                    float F0 = (powf((float)(p.x-p0.x),2) + powf((float)(p.y-p0.y),2)) - powf(height/4,2);
                    float F1 = (powf((float)(p.x-p1.x),2) + powf((float)(p.y-p1.y),2)) - powf(height/8,2);
                    float F2 = (powf((float)(p.x-p2.x),2) + powf((float)(p.y-p2.y),2)) - powf(height/4,2);
                    float F3 = (powf((float)(p.x-p3.x),2) + powf((float)(p.y-p3.y),2)) - powf(height/6,2);
                    float F4 = (powf((float)(p.x-p4.x),2) + powf((float)(p.y-p4.y),2)) - powf(height/10,2);
                    float a = 1000;
                    
                    if ((-1/a)*log(exp(-1/a*F0)+exp(-1/a*F1)+exp(-1/a*F2)+exp(-1/a*F3)+exp(-1/a*F4)) < 0) {
                        color[0] += cI[0];
                        color[1] += cI[1];
                        color[2] += cI[2];
                    }else {
                        color[0] += cO[0];
                        color[1] += cO[1];
                        color[2] += cO[2];
                    }
                }
            }
            pixmap[i++] = color[0] / powf(Num_Samples,2);
            pixmap[i++] = color[1] / powf(Num_Samples,2);
            pixmap[i] = color[2] / powf(Num_Samples,2);
        }
    }
}

void setShaded(float* cI, float* cO, float* cS)
{
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int i = (y * width + x) * 3;
            float color[3] = {0.0, 0.0, 0.0};
            float rx=rand()/RAND_MAX;
            float ry=rand()/RAND_MAX;
            for(float i=0;i<Num_Samples;i++){
                for(float j=0;j<Num_Samples;j++){
                    float x_sample=(rx+i)/Num_Samples;
                    float y_sample=(ry+j)/Num_Samples;
                    Vector2D p(x+x_sample,y+y_sample);


            if (powf((float)(p.x-width/2),2) + powf((float)(p.y-height/2),2) - powf(height/4,2) < 0) {
                float shadeR = powf(height/5,2);
                float shade = (powf((float)(p.x-width/2+30),2) + powf((float)(p.y-height/2-30),2)) - shadeR;
                if (shade <= 0) {
                    color[0] += fabsf(shade)/shadeR *cS[0] + (1 - fabsf(shade)/shadeR) * cI[0];
                    color[1] += fabsf(shade)/shadeR *cS[1] + (1 - fabsf(shade)/shadeR) * cI[1];
                    color[2] += fabsf(shade)/shadeR *cS[2] + (1- fabsf(shade)/shadeR) * cI[2];
                }else {
                    color[0] += cI[0];
                    color[1] += cI[1];
                    color[2] += cI[2];
                }
            
            }else {
                float shade = p.x/2-p.y-height;
                float shadeR = height*2;
                //printf("%f\n",shade/shadeR);
                if (shade <= 0) {
                    color[0] += fabsf(shade)/shadeR *cI[0] + (1 - fabsf(shade)/shadeR) * cO[0];
                    color[1] += fabsf(shade)/shadeR *cI[1] + (1 - fabsf(shade)/shadeR) * cO[1];
                    color[2] += fabsf(shade)/shadeR *cI[2] + (1- fabsf(shade)/shadeR) * cO[2];
                }else {
                    color[0] += cO[0];
                    color[1] += cO[1];
                    color[2] += cO[2];
                }
            }
                }
            }
            pixmap[i++] = color[0] / powf(Num_Samples,2);
            pixmap[i++] = color[1] / powf(Num_Samples,2);
            pixmap[i] = color[2] / powf(Num_Samples,2);
        }
    }
}



void draw(char * select, float* cI, float* cO, float* cS)
{
    switch (select[0]) {
        case 'c':
            setConvex(cI, cO);
            break;
        case 's':
            if (select[1] == 't') {
                setStar(cI, cO);
            }else {
                setShaded(cI, cO, cS);
            }
            break;
        case 'f':
            setFunction(cI, cO);
            break;
        case 'b':
            setBlobby(cI, cO);
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
void windowResize(int w, int h)
{
    glViewport (0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(w<=h)
        glOrtho(0,w,0,w*(GLfloat)h/(GLfloat)w,-10.0,10.0);
    else
        glOrtho(0,w*(GLfloat)w/(GLfloat)h,0,h,-10.0,10.0);
    glMatrixMode(GL_MODELVIEW);
    //printf("resize %d %d\n",w,h);
    //glOrtho(0,(w/2),0,(h/2),0,1);

}
void windowDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    //glPixelZoom(2,2);
    glRasterPos2i(0,0);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixmap);
    glFlush();
}
void processMouse(int button, int state, int x, int y)
{
//    if(state == GLUT_UP)
//        exit(0);               // Exit on mouse click.
}

void init(void)
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
    float colorOut[3] = {0xBF,0xBF,0xFF};
    float colorIn[3] = {0xFF,0X8F,0X8F};
    float colorShade[3] = {0xFF,0XFF,0XFF};

    //initialize the global variables
    width = 640;
    height = 480;
    pixmap = new unsigned char[width * height * 3];
    Num_Samples = 10;

    input = argc;
    if (input != 2)
        strcpy(select,"");
    else
        strcpy(select,argv[1]);

    while (input != 2 || (strcmp(select, "convex") != 0 && strcmp(select, "function") != 0
                          && strcmp(select, "star") != 0 && strcmp(select, "blobby") != 0
                          && strcmp(select, "circle") != 0 && strcmp(select, "shaded") != 0)) {
        printf("ERROR: Invalid input.\nPlease choose from: convex/star/function/blobby/shaded\n");
        gets(select);
        if(strstr(select," ") == 0) 
            input = 2;
        else
            input = 0;
    }
    
    draw(select,colorIn, colorOut, colorShade);
    //setPixels();
    
    
    // OpenGL Commands:
    // Once "glutMainLoop" is executed, the program loops indefinitely to all
    // glut functions.
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100); // Where the window will display on-screen.
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Project 2");
    init();
    glutDisplayFunc(windowDisplay);
    glutReshapeFunc(windowResize);
    glutMouseFunc(processMouse);
    glutMainLoop();
    
    return 0; //This line never gets reached. We use it because "main" is type int.
}

