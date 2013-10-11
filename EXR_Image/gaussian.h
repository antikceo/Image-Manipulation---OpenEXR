
#include <ImfRgbaFile.h>
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>
#include <math.h>
#include <iostream>

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

double* scale(double * pixels,int w1,int h1, double scale);