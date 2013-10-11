/*
 *
 */


#include <ImfRgbaFile.h>
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>
#include <math.h>
//#include <algorithm.h>


#include <iostream>

#define MIN(X,Y) ((X) < (Y) ? : (X) : (Y))

using namespace std;
using namespace Imf;
using namespace Imath;


void writeRgba (const char fileName[],
           const Rgba *pixels,
           int width,
                int height);

void
readRgba (const char fileName[],
          Array2D<Rgba> &pixels,
          int &width,
          int &height);

double processPoint(double ** matrix, int x, int y, double ** kernel, int direction);