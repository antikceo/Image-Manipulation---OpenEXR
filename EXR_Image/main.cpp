
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "gaussian.h"

void
writeRgba (const char fileName[],
           const Rgba *pixels,
           int width,
           int height)
{
    //
    // Write an RGBA image using class RgbaOutputFile.
    //
    //	- open the file
    //	- describe the memory layout of the pixels
    //	- store the pixels in the file
    //
    
    
    RgbaOutputFile file (fileName, width, height, WRITE_RGBA);
    file.setFrameBuffer (pixels, 1, width);
    file.writePixels (height);
}



void
readRgba (const char fileName[],
          Array2D<Rgba> &pixels,
          int &width,
          int &height)
{
    //
    // Read an RGBA image using class RgbaInputFile:
    //
    //	- open the file
    //	- allocate memory for the pixels
    //	- describe the memory layout of the pixels
    //	- read the pixels from the file
    //
    
    RgbaInputFile file (fileName);
    Box2i dw = file.dataWindow();
    
    width  = dw.max.x - dw.min.x + 1;
    height = dw.max.y - dw.min.y + 1;
    pixels.resizeErase (height, width);
    
    file.setFrameBuffer (&pixels[0][0] - dw.min.x - dw.min.y * width, 1, width);
    file.readPixels (dw.min.y, dw.max.y);
}

static double pixels( double *image, int x, int y, int width,
                           int height, int component )
{
    //boundary checking
    if( x < 0 ) {
        x = 0;
    }
    if( y < 0 ) {
        y = 0;
    }
    if( x >= width ){
        x = width - 1;
    }
    if( y >= height ) {
        y = height - 1;
    }
    
    //returns values for specific pixel components
    return image[ (y * width * 4) + (x * 4) + component ];
}




double * gauss_blur(double *src, int w, int h, int radius){
    int gWidth =  radius + 1;
    double * gauss = new double[gWidth * 2];
    
    double * out = new double[w * h * 4];
    double * temp = new double[w * h * 4];
    int kWidth;
    double sum = 0.0;
    int i, x, y;
    
    
    //build gaussian begins
    double sigma;
    double sigma2;
    double l;
    int length, n, k;
    int size  = gWidth * 2;
    
    sigma = sqrt( -(size * size) / (2.0 * log( 1.0 / 255.0 )) );
    sigma2 = (2.0 * sigma * sigma);
    l = sqrt( -sigma2 * log( 1.0 / 255.0 ) );
    n = (int)(l * 2);
    if( (n % 2) == 0 ) n++;
    length = n / 2;
    
    
    //stores gaussian values
    for( k = 0; k < (length + 1); k++ ) {
        double holder1 = exp( -(k * k) / sigma2 );
        gauss[ k ] = holder1;
    }
    
    gWidth = length + 1;
    
    
    kWidth = (gWidth * 2) - 1;
    
    for( i = 0; i < kWidth; i++ ) {
        int xpos = i - gWidth + 1;
        if( xpos < 0 ) xpos = -xpos;
        sum += gauss[ xpos ];
    }
    
    //1d gaussian blur
    for( y = 0; y < h; y++ ) {
        double *holder = temp + (y * w * 4);
        
        for( x = 0; x < w; x++ ) {
            double r = 0.0;
            double g = 0.0;
            double b = 0.0;
            double a = 0.0;
            
            for( i = 0; i < kWidth; i++ ) {
                int xpos = i - gWidth + 1;
                double op;
                if( xpos < 0 ) xpos = -xpos;
                op = gauss[ xpos ];
                r += pixels( src, x + i - (gWidth - 1), y, w, h, 0 ) * op ;
                g += pixels( src, x + i - (gWidth - 1), y, w, h, 1 ) * op;
                b += pixels( src, x + i - (gWidth - 1), y, w, h, 2 ) * op;
                a += pixels( src, x + i - (gWidth - 1), y, w, h, 3 ) * op;
            }
            holder[ (x * 4) + 0 ] = r / sum;
            holder[ (x * 4) + 1 ] = g / sum;
            holder[ (x * 4) + 2 ] = b / sum;
            holder[ (x * 4) + 3 ] = a / sum;
        }
    }
    
    //second 1d gaussian blur
    for( y = 0; y < h; y++ ) {
        double *holder = out + (y * w * 4);
        
        
        for( x = 0; x < w; x++ ) {
            double r = 0.0;
            double g = 0.0;
            double b = 0.0;
            double a = 0.0;
            
            for( i = 0; i < kWidth; i++ ) {
                int xpos = i - gWidth + 1;
                double op;
                if( xpos < 0 ) xpos = -xpos;
                op = gauss[ xpos ];
                r += pixels( temp, x, y + i - (gWidth - 1), w, h, 0 ) * op;
                g += pixels( temp, x, y + i - (gWidth - 1), w, h, 1 ) * op;
                b += pixels( temp, x, y + i - (gWidth - 1), w, h, 2 ) * op;
                a += pixels( temp, x, y + i - (gWidth - 1), w, h, 3 ) * op;
                
            }
            holder[ (x * 4) + 0 ] = r / sum;
            holder[ (x * 4) + 1 ] = g / sum;
            holder[ (x * 4) + 2 ] = b / sum;
            holder[ (x * 4) + 3 ] = a / sum;
        }
    }
    return out;
}


double* scale(double * pixels,int w1,int h1, double scale) {
    int w2 = (int)w1 * scale;
    int h2 = (int)h1 * scale;
    
    double * temp = new double[w2*h2*4] ;
    
    //int x_ratio = (int)((w1<<16)/w2) +1;
    //int y_ratio = (int)((h1<<16)/h2) +1;
    
    double px, py ;
    
    for (int i=0;i<h2;i++) {
        for (int j=0;j<w2;j++) {
            px = floor(j/scale) ;
            py = floor(i/scale) ;
            temp[((i) * w2 * 4) +((j) * 4) + 0] = pixels[(int)(((py) * w1 * 4) +((px) * 4) + 0)];
            temp[((i) * w2 * 4) +((j) * 4) + 1] = pixels[(int)(((py) * w1 * 4) +((px) * 4) + 1)];
            temp[((i) * w2 * 4) +((j) * 4) + 2] = pixels[(int)(((py) * w1 * 4) +((px) * 4) + 2)];
            temp[((i) * w2 * 4) +((j) * 4) + 3] = 1;
            //printf("Vals: %f", temp[((i) * w2 * 4) +((j) * 4) + 0]);
            //temp[(i*w2)+j] = pixels[(int)((py*w1)+px)] ;
        }                
    }                
    return temp ;
}

int main (int argc, char *argv[])
{

    try
    {
        
        cout << "reading " << argv[1] << endl;
        
        int w, h;
        Array2D<Rgba> p;
        Array2D<Rgba> pout;
        // you should test for file existence before this!
        readRgba (argv[1], p, w, h);
        
        cout << "image width height is: " << w << "  " << h << endl;
        
        
        double * src = new double[w * h * 4];
        //double * out = new double[w * h * 4];
        //printf("Str: %s", argv[3]);
        
        if (strcmp(argv[3], "-s") == 0) {
            
            //double scale_factor = 0.5;
            double scale_factor = atof(argv[4]);
            int w2 = (int)(w * scale_factor);
            int h2 = (int)(h * scale_factor);
            
            //printf("h2: %d\n", h2);
            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    
                    Rgba &px = p[y][x];  // get the pixel we are interested in
                    src[((y) * w * 4) + ((x) * 4) +0] = px.r;
                    src[((y) * w * 4) + ((x) * 4) +1] = px.g;
                    src[((y) * w * 4) + ((x) * 4) +2] = px.b;
                    src[((y) * w * 4) + ((x) * 4) +3] = 1;
                }
            }
            
            double * outputImg = scale(src, w, h, scale_factor);
            //readRgba (argv[1], p, w2, h2);
        
            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    
                    Rgba &px = p[y][x];
                    px.r = 0;
                    px.g = 0;
                    px.b = 0;
                    px.a = 0;
                    
                }
            }
            
            for (int y = 0; y < h2; ++y) {
                for (int x = 0; x < w2; ++x) {
                    
                    Rgba &px = p[y][x];
                    px.r = outputImg[((y) * w2 * 4) +((x) * 4)  +0];
                    px.g = outputImg[((y) * w2 * 4) +((x) * 4)  +1];
                    px.b = outputImg[((y) * w2 * 4) +((x) * 4)  +2];
                    px.a = 1;
                    
                }
            }
            cout << "writing output image" << endl;
            
            writeRgba (argv[2], &p[0][0], w, h);
            
            cout << "done." << endl;
        }
        
        
        else if (strcmp(argv[3], "-b") == 0) {
            int itr = 0;
            int radius = atoi(argv[4]);
            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    
                    Rgba &px = p[y][x];  // get the pixel we are interested in
                    src[((y) * w * 4) +((x) * 4) + 0] = px.r;
                    
                    src[((y) * w * 4) + ((x) * 4) +1] = px.g;
                    src[((y) * w * 4) + ((x) * 4) +2] = px.b;
                    src[((y) * w * 4) + ((x) * 4) +3] = 1;
                }
            }
            Rgba &px = p[0][57];
            double * outputImg = gauss_blur(src, w, h, radius);
            double hold = px.r;
            
            
            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    Rgba &px = p[y][x];
                    px.r = outputImg[((y) * w * 4) +((x) * 4)  +1];
                    px.g = outputImg[((y) * w * 4) +((x) * 4)  +2];
                    px.b = outputImg[((y) * w * 4) +((x) * 4)  +3];
                    px.a = 1;
                    
                }
            }
            cout << "writing output image" << endl;
            
            writeRgba (argv[2], &p[0][0], w, h);
            
            cout << "done." << endl;
        }
        else{
            printf("Invalid Arguement!");
        }

    }
    
    catch (const std::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return 1;
    }
    
    return 0;
}



