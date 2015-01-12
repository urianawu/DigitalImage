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
#include "colorSpaceConversion.h"

#ifdef __APPLE__
#  include <GLUI/glui.h>
#else
#  include <GL/glui.h>
#endif

using namespace std;

// =============================================================================
// These variables will store the input ppm image's width, height, and color
// =============================================================================

int width, height, level, mWidth, mHeight, wWidth, wHeight;
unsigned char *fore, *composite, *back, *minions, *wreckit, *newcomposite;

#define N 3  //filter size

int S = 100;  //samples
int CY[255];
int cxLevel = 0;
int switchFunction = 0;
bool inverse = false;
bool greenScreen = false;

GLint mainWindow, originalWindow, backWindow;  //windows
GLUI_RadioGroup *ot_group, *ground_group;
int radiogroup_item_id, fore_id;

float f(float c0, float c1, int function)
{
    switch (function) {
        case 0:
            //over
            return c1;
            break;
        case 1:
            //multiplication
            return (c0 * c1);
            break;
        case 2:
            //substraction
            return fabsf(c1 - c0);
            break;
        case 3:
            //max
            return max(c1,c0);
            break;
        case 4:
            //min
            return min(c1,c0);
            break;
        default:
            break;
    }
    return 0;
}

void composition(int mode, int function)
{
    //foreground c1,a1
    //background c0,a0
    for(int y = 0; y < height ; y++) {
        for(int x = 0; x < width; x++) {
            int i = (y * width + x) * 3;
            float r,g,b,r0,g0,b0,r1,g1,b1,h,s,v,h0,s0,v0,h1,s1,v1;
            float a,a1,a0;
            a1 = 0.5;
            a0 = 0.7;
            
            //calculate normalized rgb color
            r1 = a1 * (float)fore[i++]/level;
            g1 = a1 * (float)fore[i++]/level;
            b1 = a1 * (float)fore[i]/level;
            
            i = (y * width + x) * 3;
            r0 = a0 * (float)back[i++]/level;
            g0 = a0 * (float)back[i++]/level;
            b0 = a0 * (float)back[i]/level;
            
            if (inverse) {
                float temp = 0;
                temp = r1;
                r1 = r0;
                r0 = temp;
                
                temp = g1;
                g1 = g0;
                g0 = temp;
                temp = b1;
                b1 = b0;
                b0 = temp;
            }
            if (mode == 0) {
                //rgb
                r = a1 * f(r0,r1,function) + (1 - a1) * r0;
                g = a1 * f(g0,g1,function) + (1 - a1) * g0;
                b = a1 * f(b0,b1,function) + (1 - a1) * b0;
                a = a1 + (1 -a1) * a0;
                
                if (r > 1) {
                    r = 1;
                }
                if (g > 1) {
                    g = 1;
                }
                if (b > 1) {
                    b = 1;
                }
                
                i = (y * width + x) * 3;
                composite[i++] = r * (float)level / a;
                composite[i++] = g * (float)level / a;
                composite[i] = g * (float)level / a;

            }else {
                //hsv
                
            }
        }
    }
}

void selectOperation(int operation)
{
    switch (operation) {
        case 0:
            //rgb over
            composition(0,0);
            break;
        case 1:
            //hsv over
            composition(1,0);
            break;
        case 2:
            //rgb Multiplication
            composition(0,1);
            break;
        case 3:
            //hsv Multiplication
            composition(1,1);
            break;
        case 4:
            //rgb substaction
            composition(0,2);
            break;
        case 5:
            //hsv substaction
            composition(1,2);
            break;
        case 6:
            //rgb max
            composition(0,3);
            break;
        case 7:
            //hsv max
            composition(1,3);
            break;
        case 8:
            //rgb min
            composition(0,4);
            break;
        case 9:
            //hsv min
            composition(1,4);
            break;
        case 10:
            //green screening
            //composite(0,);
            break;


        default:
            break;
    }
}

void greenScreening()
{
    
    for(int y = 0; y < (wHeight) ; y++) {
        for(int x = 0; x < wWidth; x++) {
            int i = (y * wWidth + x) * 3;
            float r1,g1,b1,h1, s1, v1,r,g,b,r0,g0,b0;
            float a0 = 1;
            float a, a1;

            r1 =  (float)minions[i++]/level;
            g1 =  (float)minions[i++]/level;
            b1 =  (float)minions[i]/level;
            
            colorSpaceConversion conv;
            conv.RGBtoHSV(r1, g1, b1, &h1, &s1, &v1);
            if (h1<129 && h1> 115) {
                a1 = 0;
            }else
                a1 = 1;

            i = (y * wWidth + x) * 3;
            r1 =  a1 * (float)minions[i++]/level;
            g1 =  a1 * (float)minions[i++]/level;
            b1 =  a1 * (float)minions[i]/level;
            
            i = (y * wWidth + x) * 3;
            r0 = a0 * (float)wreckit[i++]/level;
            g0 = a0 * (float)wreckit[i++]/level;
            b0 = a0 * (float)wreckit[i]/level;
            
            i = (y * wWidth + x) * 3;
            r = a1 * r1 + (1 - a1) * r0;
            g = a1 * g1 + (1 - a1) * g0;
            b = a1 * b1 + (1 - a1) * b0;
            a = a1 + (1 -a1) * a0;

            
            i = (y * wWidth + x) * 3;
            newcomposite[i] = r * (float)level/a;
            i++;
            newcomposite[i] = g * (float)level/a;
            i++;
            newcomposite[i] = b * (float)level/a;
        }
    }
}
// =============================================================================
// setPixels()
//
// This function stores the RGB values of each pixel to "pixmap."
// Then, "glutDisplayFunc" below will use pixmap to display the pixel colors.
// =============================================================================
void initialize(){
    for (int i = 0; i< width * height *3; i++) {
        composite[i] = 0;
    }

}

void setPixels()
{
    ppmview foreimage;
    ppmview backimage;
    ppmview minionsimage;
    ppmview wreckitimage;
    /**
     change absolute path of the images here!!
     */
    std::string fname = "/Users/Uriana/Desktop/DigitalImage/images_start/screen10.ppm";

    char *ffilename = new char[fname.length() + 1];
    std::strcpy(ffilename, fname.c_str());
    
    foreimage.readPPM(ffilename);
    width = foreimage.width;
    height = foreimage.height;
    level = foreimage.maxcolor;
    
    std::string bname = "/Users/Uriana/Desktop/DigitalImage/images_end/screen10.ppm";
    
    char *bfilename = new char[bname.length() + 1];
    std::strcpy(bfilename, bname.c_str());
    
    backimage.readPPM(bfilename);

    std::string mname = "/Users/Uriana/Desktop/DigitalImage/minions.ppm";
    char *mfilename = new char[mname.length() + 1];
    std::strcpy(mfilename, mname.c_str());
    
    minionsimage.readPPM(mfilename);
    wWidth = minionsimage.width;
    wHeight = minionsimage.height;
    
    std::string wname = "/Users/Uriana/Desktop/DigitalImage/wreckit.ppm";
    char *wfilename = new char[wname.length() + 1];
    std::strcpy(wfilename, wname.c_str());
    
    wreckitimage.readPPM(wfilename);
    wWidth = wreckitimage.width;
    wHeight = wreckitimage.height;
    
    composite = new unsigned char[width * height * 3];
    fore = new unsigned char[width * height * 3];
    back = new unsigned char[width * height * 3];
    minions = new unsigned char[mWidth * mHeight * 3];
    wreckit = new unsigned char[wWidth * wHeight * 3];
    newcomposite = new unsigned char[wWidth * wHeight * 3];

    fore = foreimage.pixmap;
    back = backimage.pixmap;
    minions = minionsimage.pixmap;
    wreckit = wreckitimage.pixmap;
    initialize();

    
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
    if (greenScreen) {
        glPixelZoom(0.8,0.8);
        glDrawPixels(wWidth, wHeight, GL_RGB, GL_UNSIGNED_BYTE, minions);
    }else {
        glPixelZoom(0.4,0.4);
        glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, fore);
    }
    glutSwapBuffers();
}

static void BwindowDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1,1,1,1); // Set background color.
    glRasterPos2i(0,0);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    if (greenScreen) {
        glPixelZoom(0.8,0.8);
        glDrawPixels(wWidth, wHeight, GL_RGB, GL_UNSIGNED_BYTE, wreckit);

    }else {
        glPixelZoom(0.4,0.4);
        glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, back);
    }
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
    glPixelZoom(0.8,0.8);
    if (greenScreen) {
        glDrawPixels(wWidth, wHeight, GL_RGB, GL_UNSIGNED_BYTE, newcomposite);
    }else {
        glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, composite);
    }
    glutSwapBuffers();
}

static void processMouse(int button, int state, int x, int y)
{
    
}


static void init(void)
{
    glClearColor(1,1,1,1); // Set background color.
   
}

void idle ()
{
    glutSetWindow(mainWindow);
    //glutSetWindow(originalWindow);
    glutPostRedisplay();

}

void glui_callback(int control)
{
    //printf("%d",radiogroup_item_id);

    switch (control) {
        case 1:
            switch (fore_id) {
                case 0:
                    initialize();
                    inverse = false;
                    selectOperation(ot_group->get_int_val()*2);
                    break;
                case 1:
                    initialize();
                    inverse = true;
                    selectOperation(ot_group->get_int_val()*2);

                default:
                    break;
            }
            break;
        case 2:
            switch (radiogroup_item_id) {
                case 0:

                    initialize();
                    selectOperation(0);

                    break;
                case 1:
                    initialize();
                    selectOperation(2);

                    break;
                case 2:
                    
                    initialize();
                    selectOperation(4);

                    break;

                case 3:
                    initialize();
                    selectOperation(6);

                    break;
                case 4:
                    initialize();
                    selectOperation(8);

                    break;
                case 5:
                    initialize();
                    greenScreen = true;

                    break;
                    
                default:
                    break;
            }

            break;
            
        default:
            break;
    }
}
// =============================================================================
// main() Program Entry
// =============================================================================
int main(int argc, char *argv[])
{

    setPixels();
    initialize();
    //greenScreen = true;
    //greenScreening();
    selectOperation(0);
    // OpenGL Commands:
    // Once "glutMainLoop" is executed, the program loops indefinitely to all
    // glut functions.
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100); // Where the window will display on-screen.
    if (greenScreen) {
        glutInitWindowSize(wWidth*0.8, wHeight*0.8);
    }
    else {
        glutInitWindowSize(width*0.4, height*0.4);
    }
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    originalWindow = glutCreateWindow("Tree Image");
    init();
    glutReshapeFunc(OwindowResize);
    glutDisplayFunc(OwindowDisplay);
    
    if (greenScreen) {
        glutInitWindowSize(wWidth*0.8, wHeight*0.8);
    }
    glutInitWindowPosition(100, 100+height*0.4); // Where the window will display on-screen.
    backWindow = glutCreateWindow("Girl Image");
    glutReshapeFunc(OwindowResize);
    glutDisplayFunc(BwindowDisplay);
    
    glutInitWindowPosition(100+width*0.4, 100); // Where the window will display on-screen.
    if (greenScreen) {
        glutInitWindowSize(wWidth*0.8, wHeight*0.8);
    }else {
        glutInitWindowSize(width*0.8, height*0.8);
    }
    mainWindow = glutCreateWindow("Composite Image");
    glutReshapeFunc(windowResize);
    glutDisplayFunc(windowDisplay);
    glutMouseFunc(processMouse);
    
    
    
    GLUI * glui_window;
    
    //  Create GLUI window
    
    
    glui_window = GLUI_Master.create_glui ("Options",0, 100+width*0.4+width*0.8,100);
    
    GLUI_Panel *ground_panel = glui_window->add_panel ("Foreground image");
    ground_group = glui_window->add_radiogroup_to_panel(ground_panel,&fore_id, 1, glui_callback);
    glui_window->add_radiobutton_to_group( ground_group, "tree" );
    glui_window->add_radiobutton_to_group( ground_group, "girl" );
    ground_group->set_int_val(0);

    //  Create radio button group
    GLUI_Panel *ot_panel = glui_window->add_panel ("Operations");

    ot_group = glui_window->add_radiogroup_to_panel(ot_panel,&radiogroup_item_id, 2, glui_callback);
    glui_window->add_radiobutton_to_group( ot_group, "Over" );
    glui_window->add_radiobutton_to_group( ot_group, "Multiplication" );
    glui_window->add_radiobutton_to_group( ot_group, "Subtraction" );

    glui_window->add_radiobutton_to_group( ot_group, "Max" );
    glui_window->add_radiobutton_to_group( ot_group, "Min" );

    ot_group->set_int_val(0);

    glui_window->add_button("Quit",0, (GLUI_Update_CB)exit);
    
    glui_window->sync_live();
    GLUI_Master.sync_live_all();
    GLUI_Master.set_glutIdleFunc (idle);
    glutMainLoop();
    
    return 0; //This line never gets reached. We use it because "main" is type int.
}

