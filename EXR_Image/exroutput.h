/**
 * Copyright (C) 2003 Billy Biggs <vektor@dumbterm.net>.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef EXROUTPUT_H_INCLUDED
#define EXROUTPUT_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
    
    /**
     * Example usage:
     *
     * exroutput_t *exrout = exroutput_new( "myimage.exr", width, height );
     *
     * for( i = 0; i < height; i++ ) {
     *     exroutput_scanline( exrout, myimage + (i*bytesperline) );
     * }
     *
     * exroutput_delete( exrout );
     */
    
    typedef struct exroutput_s exroutput_t;
    
    /**
     * Creates a new EXR output file at the given width and height.
     */
    exroutput_t *exroutput_new( const char *filename, int width, int height );
    
    /**
     * Closes the EXR file, finishing the write.
     */
    void exroutput_delete( exroutput_t *exroutput );
    
    /**
     * Writes a scanline (of the appropriate width) to the exr file.  Scanlines
     * should be provided from top to bottom of the image, and must include all
     * four channels RGBA in that order.
     */
    void exroutput_scanline( exroutput_t *exroutput, double *scanline );
    
#ifdef __cplusplus
};
#endif
#endif /* EXROUTPUT_H_INCLUDED */
