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
#include "NTmatrix.h"

#define PI 3.1415

using namespace std;

// =============================================================================
// These variables will store the input ppm image's width, height, and color
// =============================================================================

int inputwidth, inputheight, level, outputwidth, outputheight;
unsigned char *in, *out;

NTmatrix final(3, 3, 'I');  //final transformation matrix initialized as 3*3 identity matrix
Vector2D lb, rb, lt, rt;    //corners for output image
int Samples = 3;  //samples

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
    inputwidth = input.width;
    inputheight = input.height;
    level = input.maxcolor;
    
    
    in = new unsigned char[inputwidth * inputheight * 3];
    in = input.pixmap;
    
}


void transform(char operation[], float p1, float p2 )
{
    NTmatrix temp(3, 3);
    
    
        switch (operation[0]) {
            case 'r':
            {
                double trans[9] = {cosf(p1 * PI/180), sinf(p1* PI/180), 0, -sinf(p1* PI/180), cosf(p1* PI/180), 0, 0, 0, 1};
                temp.assign(trans);
                break;
            }
            case 's':
            {
                double trans[9] = {p1, 0, 0, 0, p2, 0, 0, 0, 1};
                temp.assign(trans);
                break;
            }
            case 't':
            {
                double trans[9] = {1, 0, 0, 0, 1, 0, p1, p2, 1};
                temp.assign(trans);
                outputheight+=(int)p2*2;
                outputwidth+=(int)p1*2;
                break;
            }
            case 'f':
            {
                int flipx = 1;
                int flipy = 1;
                if (p1 == 1) {
                    flipx = -1;
                }
                if (p2 == 1) {
                    flipy = -1;
                }
                double trans[9] = {(double)flipx, 0, 0, 0, (double)flipy, 0, 0, 0, 1};
                temp.assign(trans);
                break;
            }
            case 'h':
            {
                double trans[9] = {1, p2, 0, p1, 1, 0, 0, 0, 1};
                temp.assign(trans);
                break;
            }
            case 'p':
            {
                double trans[9] = {1, 0, p1, 0, 1, p2, 0, 0, 1};
                temp.assign(trans);
                break;
            }
            case 'o':
            {
                double trans[9] = {0.6,0.2,0,0.4,0.5,0,10,30,1};
                temp.assign(trans);
                break;
            }
            default:
            {
                double trans[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
                temp.assign(trans);
                break;
            }
        }
    //temp.printMat();
    final = temp * final;
    //final.printMat();

}

void transformCorner()
{
    NTmatrix c00(3,1), c10(3,1), c01(3,1), c11(3,1);
    double temp[3] = {-(double)inputwidth/2, -(double)inputheight/2, 1};
    c00.assign(temp);
    temp[0] = inputwidth/2;
    c10.assign(temp);
    temp[0] = -inputwidth/2;
    temp[1] = inputheight/2;
    c01.assign(temp);
    temp[0] = inputwidth/2;
    c11.assign(temp);
    
    NTmatrix new00(3,1), new10(3,1), new01(3,1), new11(3,1);
    new00 = final * c00;
    new10 = final * c10;
    new01 = final * c01;
    new11 = final * c11;

    lb.Set(new00.vect[0]/new00.vect[2]+inputwidth/2,new00.vect[1]/new00.vect[2]+inputheight/2);
    rb.Set(new10.vect[0]/new00.vect[2]+inputwidth/2,new10.vect[1]/new00.vect[2]+inputheight/2);
    lt.Set(new01.vect[0]/new00.vect[2]+inputwidth/2,new01.vect[1]/new00.vect[2]+inputheight/2);
    rt.Set(new11.vect[0]/new00.vect[2]+inputwidth/2,new11.vect[1]/new00.vect[2]+inputheight/2);

}

void initializeOutput(){
    //printf("%f %f %f %f\n %f %f %f %f\n",lt.y,rt.y,lb.y,rb.y,lt.x,rt.x,lb.x,rb.x);
    outputheight += max(max(max(lt.y,rt.y),rb.y),lb.y);
    outputwidth += max(max(max(lt.x,rt.x),rb.x),lb.x);
    int minheight = min(min(min(lt.y,rt.y),rb.y),lb.y);
    int minwidth = min(min(min(lt.x,rt.x),rb.x),lb.x);
    if (minheight < 0) {
        outputheight -= minheight;
    }
    if (minwidth < 0) {
        outputwidth -= minwidth;
    }
    
    out = new unsigned char[outputwidth * outputheight * 3];
    for (int i = 0; i< outputwidth * outputheight *3; i++) {
        out[i] = 0;
    }

}

void InverseWarping()
{
    NTmatrix invMatrix;
    
    invMatrix = final.inverse();
    //final.printMat();
    //invMatrix.printMat();
    
    for(int y = 0; y < outputheight ; y++) {
        for(int x = 0; x < outputwidth; x++) {
            int pix = (y * outputwidth + x) * 3;
            float color[3] = {0.0, 0.0, 0.0};
            float rx=rand()/RAND_MAX;
            float ry=rand()/RAND_MAX;
            for(float i = 0;i<Samples;i++){
                for(float j = 0;j<Samples;j++){
                    float x_sample=(rx+i)/Samples;
                    float y_sample=(ry+j)/Samples;
                    Vector2D p(x+x_sample,y+y_sample);
                    
                    NTmatrix outAxis(3,1);
                    NTmatrix originAxis(3,1);
                    int inputx, inputy;
                    double axis[3] = {(double)p.x - outputwidth/2,(double)p.y - outputheight/2,1};
                    outAxis.assign(axis);
                    originAxis = invMatrix * outAxis;
                    inputx = (int)(originAxis.vect[0]/originAxis.vect[2] + inputwidth/2);
                    inputy = (int)(originAxis.vect[1]/originAxis.vect[2] + inputheight/2);
                    
                    int inputi = (inputy * inputwidth + inputx) * 3;
                    if (inputx >= 0 && inputx < inputwidth) {
                        if (inputy >= 0 && inputy < inputheight) {
                            color[0] += in[inputi++];
                            color[1] += in[inputi++];
                            color[2] += in[inputi];
                        }
                    }
                }
            }
            
            out[pix++] = color[0] / powf(Samples,2);
            out[pix++] = color[1] / powf(Samples,2);
            out[pix] = color[2] / powf(Samples,2);
        }
    }
}

void bilinearWarping()
{
    float x0 = lb.x/outputwidth;
    float x1 = lt.x/outputwidth;
    float x2 = rt.x/outputwidth;
    float x3 = rb.x/outputwidth;
    
    float y0 = lb.y/outputheight;
    float y1 = lt.y/outputheight;
    float y2 = rt.y/outputheight;
    float y3 = rb.y/outputheight;
    
    float a0 =x0;
    float a1 =x3-x0;
    float a2 =x1-x0;
    float a3 =x2-x1-x3-x0;
    float b0 =y0;
    float b1 =y3-y0;
    float b2 =y1-y0;
    float b3 =y2-y1-y3-y0;
    
    for(int y = 0; y < outputheight ; y++) {
        for(int x = 0; x < outputwidth; x++) {
            int pix = (y * outputwidth + x) * 3;
            float color[3] = {0.0, 0.0, 0.0};
            float rx=rand()/RAND_MAX;
            float ry=rand()/RAND_MAX;
            for(float i = 0;i<Samples;i++){
                for(float j = 0;j<Samples;j++){
                    float x_sample=(rx+i)/Samples;
                    float y_sample=(ry+j)/Samples;
                    Vector2D p((x+x_sample)/((float)inputwidth),(y+y_sample)/((float)inputheight));
                    
                    float c0 = a1*(b0 - p.y) + b1*(p.x - a0);
                    float c1 = a3*(b0 - p.y) + b3*(p.x - a0) + a1*b2 -a2*b1;
                    float c2 = a3*b2 - a2*b3;
                    
                    float v1 = (-c1 + sqrt((c1*c1)-4*c2*c0))/(2*c2);
                    float v2 = (-c1 - sqrt((c1*c1)-4*c2*c0))/(2*c2);
                    float u1 = (p.x-a0 -a2*v1)/ (a1 + a3*v1);
                    float u2 = (p.x-a0 -a2*v2)/ (a1 + a3*v2);

                    float u,v;
                    if (v1>0 && v1<1 && u1>0 && u1>1) {
                        v = v1;
                        u = u1;
                    }
                    if (v2>0 && v2<1 && u2>0 && u2<1) {
                        v = v2;
                        u = u2;
                    }
                    //printf("%f %f\n",u,v);
                    int inputx, inputy;
                    inputx = 0 + (inputwidth - 0)*u;
                    inputy = 0 + (inputheight - 0)*v;
                    inputx = (int)(inputx + 0.5);
                    inputy = (int)(inputy + 0.5);
                    
                    int inputi = (inputy * inputwidth + inputx) * 3;
                    color[0] += in[inputi++];
                    color[1] += in[inputi++];
                    color[2] += in[inputi];
                }
            }
            
            out[pix++] = color[0] / powf(Samples,2);
            out[pix++] = color[1] / powf(Samples,2);
            out[pix] = color[2] / powf(Samples,2);
            
        }
    }
}
void outputImage(char *outfile)
{
    ppmview output;
    output.width = outputwidth;
    output.height = outputheight;
    output.maxcolor = level;
    output.pixmap = out;
    if (outfile != NULL) {
        output.writePPM(outfile,0);
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
    float parameter1, parameter2 = 0;
    bool bilinear = false;  // false:projective  true:bilinear
    int transNum = 0;   //# of transformation
    
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

    //transformation control
    printf("Enter the projective warp(s) you want to perform:\n"
           "r theta  - rotate theta degree (CCW).\n"
           "s sx sy  - scale.\n"
           "t dx dy  - translate.\n"
           "f xf yf  - mirror. (1 = flip, 0 = no flip)\n"
           "h hx hy  - shear.\n"
           "p px py  - perspective.\n"
           "o        - own transformation\n"
           "-b       - change to bilinear mode\n"
           "d        - done.\n");
           
    while (1) {
        printf("Enter warp:");
        scanf("%s", operation);
        while (operation[0] != 'r' && operation[0] != 's' && operation[0] != 't' && operation[0] != 'f' && operation[0] != 'h' && operation[0] != 'p' && operation[0] != 'o' && operation[0] != 'd' && operation[0] != '-') {
            printf("Invalid input, please try again.\n");
            printf("Enter warp:");
            scanf("%s", operation);
        }
        
        if (strcmp(operation,"-b") == 0 && transNum == 0) {
            bilinear = !bilinear;
            printf("Mode changed successfully.\n");
            if (bilinear) {
                printf("Now it's bilinear warping.\n");
            }else {
                printf("Now it's projective warping.\n");
            }
        }else if (strcmp(operation,"-b") == 0 && transNum != 0) {
            printf("Can't change mode now. Change it before any transformation applied.\n");
        }

        if (strcmp(operation, "d") != 0 && strcmp(operation, "-b") != 0 && strcmp(operation, "o") != 0) {
            scanf("%f", &parameter1);
        }else if ( strcmp(operation, "d") == 0){
            break;
        }
        
        if (strcmp(operation,"r") != 0 && strcmp(operation, "-b") != 0 && strcmp(operation, "o") != 0) {
            scanf("%f", &parameter2);
        }

        transform(operation, parameter1, parameter2);
        transNum++;
    }
    
    transformCorner();
    initializeOutput();
    
    if (bilinear) {
        bilinearWarping();
    }else{
        InverseWarping();
    }
    if (argc == 3) {
        outputImage(outFile);
        printf("[Your program will display and save the warped image.]\n");
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

