/*
    Copyright 2018 Electronic Key Smith
	 www.ekeysmith.com
	  *
	  Version: 0.1
	   *
	   * Edit history:
	   * Gintec : September 6th creation

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
*/


#include "VCG2DLib.h"
#include "math.h"

namespace VCG2D {
namespace Lines{
namespace Fill {


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



void draw_line_fill(VCG2DVAR &x1, VCG2DVAR &y1,
							VCG2DVAR &x2, VCG2DVAR &y2, VERSION_BUFFER_TYPE &version

								)
{
	dir[0] = x2 - x1;
	dir[1] = y2 - y1;

	/// normalize it
	length = sqrt(1.0/(dir[0]*dir[0]+dir[1]*dir[1])*0.9999999);

	dirl[0] = dir[0]*length;//*0.9999999;
	dirl[1] = dir[1]*length;//*0.9999999;

	pos[0] = x1;
	pos[1] = x2;

	VERSION_BUFFER_TYPE *pixels_v = g_current_screen_buffer_version;
	bufferoffset = 0;

	for(VCG2DVAR start = 0; start<=1.0; start+=length)
	{
		pos[0] += dirl[0];
		pos[1] += dirl[1];

		bufferoffset = (pos[0] + pos[1]*g_current_screen_width);
		pixels_v[bufferoffset] = 1;

	}
}



}
}
}
