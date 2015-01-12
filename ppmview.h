#include <cstdlib>
#include <iostream>
#include <sstream>
//#include <GLUT/glut.h>
//#include <openGL/glu.h>
#include <cstring>

using namespace std;
class ppmview
{
public:
    int width, height, maxcolor;
    unsigned char *pixmap;
    int readPGM (char * filename);
    int readPPM (char * filename);
    void writePPM(char* outfile, int A);
    void writePGM(char* outfile, int A);

};
