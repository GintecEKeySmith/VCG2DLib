/***
    Copyright 2018 Electronic Key Smith
	 www.ekeysmith.com
	  * 
	  Version: 0.1
	   * 
	   * Edit history: 
	   * Gintec : September 6th creation
MIT License
 * 
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * The library is based upon a paper:
 * 
 * http://www.academia.edu/36226207/A_novel_approach_to_2D_and_3D_computer_graphics._Reducing_pipe_intermingling
 * 
 * 
*/





#include "VCG2DLib.h"
#include "VCG2DLib_fill.h"
#include "VCG2DLib_gradients.h"

class dir;
namespace VCG2D {
namespace FillSpline {

	
VCG2DVAR dir[2];
VCG2DVAR dirl[2];
VCG2DVAR length;
VCG2DVAR pos[2];
VCG2DVAR colgrads[SCREEN_INTERNAL_PIXELSIZE];

unsigned char *pixels;
unsigned char *cpixel;
VERSION_BUFFER_TYPE *pixels_v;
int *tcvindex;
VCG2DVAR _vd;
VCG2DVAR _v;
float _tallinv;
unsigned char col[4];
VCG2DVAR colVgrad[4];
int bufferoffset;
int maxiters;

int ipos[2];

}}