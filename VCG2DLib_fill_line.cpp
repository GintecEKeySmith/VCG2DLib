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

#include <math.h>       /* sqrt */

class dir;
namespace VCG2D {
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
int maxiters;

int ipos[2];


void fill_line_onecolor(VCG2DVAR &x1, VCG2DVAR &y1, 
							VCG2DVAR &x2, VCG2DVAR &y2, 
								color4_t &color, 
									VERSION_BUFFER_TYPE &versionid)
{

	dir[0] = x2 - x1;
	dir[1] = y2 - y1;

	if(dir[1] >= 0)
		return;
		
	/// normalize it
	length = sqrt(1.0/(dir[0]*dir[0]+dir[1]*dir[1])*0.9999999);

	dirl[0] = dir[0]*length;//*0.9999999; 
	dirl[1] = dir[1]*length;//*0.9999999;
	
	pos[0] = x1;
	pos[1] = y1;
	
	for(VCG2DVAR start = 0; start<=1.0; start+=length)
	{
		pos[0] += dirl[0]; //dir[0]*length;
		pos[1] += dirl[1]; //dir[1]*length;

		/// set the pixel
		bufferoffset = ((int)pos[0] + (int)(pos[1])*g_current_screen_width);

		maxiters = (int)g_current_screen_right - (int)pos[0];
		VCG2D::Fill::fill_xlinepos(bufferoffset, maxiters, versionid, color);		
	}	
}


void fill_line_onecolor_gradient(
							VCG2DVAR &x1, VCG2DVAR &y1,
							VCG2DVAR &x2, VCG2DVAR &y2,
							Gradients::gradient2d_t &colorgrad,
							VERSION_BUFFER_TYPE &version
							)
{

	dir[0] = x2 - x1;
	dir[1] = y2 - y1;
	
		if(dir[1] >= 0)
		return;
	/// normalize it
	length =sqrt(1.0/(dir[0]*dir[0]+dir[1]*dir[1])*0.9999999);

	dirl[0] = dir[0]*length;//*0.9999999; 
	dirl[1] = dir[1]*length;//*0.9999999;
	
	pos[0] = x1;
	pos[1] = y1;

	
	for(VCG2DVAR start = 0; start<=1.0; start+=length)
	{
		pos[0] += dirl[0]; //dir[0]*length;
		pos[1] += dirl[1]; //dir[1]*length;

		ipos[0] = (int)pos[0];
		ipos[1] = (int)pos[1];
		
		/// set the pixel
		bufferoffset = (ipos[0] + (ipos[1])*g_current_screen_width);

		maxiters = g_current_screen_right - ipos[0];
		/*
		 * void fill_xlinepos_gradient(int &offset, int &maxiters,
								CGLCOLOR4 &x1, CGLCOLOR4 &y1,
								CG2DVAR &x2, VCG2DVAR &y2,
								VERSION_BUFFER_TYPE &version, 
								CG2D::Gradients::gradient2d_t &color);
		 * */
		/*
		 * void fill_xlinepos_gradient(int &offset, int &maxiters,
								CGLCOLOR4 &xpos, CGLCOLOR4 &ypos,
								VERSION_BUFFER_TYPE &version, 
								CG2D::Gradients::gradient2d_t &colorgrad)
		 * 
		 * */
		
		VCG2D::Fill::fill_xlinepos_gradient(bufferoffset, maxiters, pos[0], pos[1],
												 version, colorgrad);		
	}	
}

}}