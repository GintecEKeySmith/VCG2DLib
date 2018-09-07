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
#include "math.h"
#include "VCG2DLib_fill.h"

namespace VCG2D {
namespace Lines{


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

/// in screen coordinates !
void draw_line_noblending(int screenid, VCG2DVAR &x1, VCG2DVAR &y1, VCG2DVAR &x2, VCG2DVAR &y2,
						VCG2DVAR (&col1)[SCREEN_INTERNAL_PIXELSIZE],
						VCG2DVAR (&col2)[SCREEN_INTERNAL_PIXELSIZE],
						VCG2DVAR &versionid)
{

	dir[0] = x2 - x1;
	dir[1] = y2 - y1;
	/// normalize it
	length =sqrt(1.0/(dir[0]*dir[0]+dir[1]*dir[1])*0.9999999);

	dirl[0] = dir[0]*length;//*0.9999999;
	dirl[1] = dir[1]*length;//*0.9999999;

	pos[0] = x1;
	pos[1] = x2;


	colgrads[0] = (col2[0]-col1[0])*length;
	colgrads[1] = (col2[1]-col1[1])*length;
	colgrads[2] = (col2[2]-col1[2])*length;
	colgrads[3] = (col2[3]-col1[3])*length;
	colgrads[4] = (col2[4]-col1[4])*length;
	colgrads[5] = (col2[5]-col1[5])*length;
	colgrads[6] = (col2[6]-col1[6])*length;
	colgrads[7] = (col2[7]-col1[7])*length;

	col[0] = col1[0];
	col[1] = col1[1];
	col[2] = col1[2];
	col[3] = col1[3];
	col[4] = col1[4];
	col[5] = col1[5];
	col[6] = col1[6];
	col[7] = col1[7];

	pixels = g_current_screen_buffer;

	//return;

	for(VCG2DVAR start = 0; start<=1.0; start+=length)
	{
		pos[0] += dirl[0];
		pos[1] += dirl[1];

		/// set the pixel
		bufferoffset = (int)((pos[0] + pos[1]*g_current_screen_width)*g_screen_buffer_pixel_size);

		cpixel = &(pixels[bufferoffset]);
		_v = cpixel[7];
		_vd = versionid - _v;

		/// this line sucks !!
		if(_vd >= 1.0)
			_vd = 1.0;
		else continue;

		_tallinv = 1.0/(cpixel[3]+col[3]);

//// r1,g1,b1,t1,i1,si1,bl1, r2,g2,b2,t2,i2,si2,bl2, r3,g3,b3,t3,i3,si3,bl3, vdin, vin, vdout, tall
		///STANDART_COLOR_MIX

		STANDART_COLOR_MIX(col[0], col[1], col[2], col[3], col[4], col[5], col[6],
							cpixel[0], cpixel[1], cpixel[2], cpixel[3], cpixel[4], cpixel[5], cpixel[6],
							 cpixel[0], cpixel[1], cpixel[2], cpixel[3], cpixel[4], cpixel[5], cpixel[6],
							  _vd, _v, cpixel[7], _tallinv
							)
	}
}



/// in screen coordinates !
void draw_line_onecolor(VCG2DVAR &x1, VCG2DVAR &y1, VCG2DVAR &x2, VCG2DVAR &y2,
						VCG2DVAR (&col1)[SCREEN_INTERNAL_PIXELSIZE_COLORANDALPHA],
						VCG2DVAR &versionid)
{

	dir[0] = x2 - x1;
	dir[1] = y2 - y1;
	/// normalize it
	length =sqrt(1.0/(dir[0]*dir[0]+dir[1]*dir[1])*0.9999999);

	dirl[0] = dir[0]*length;
	dirl[1] = dir[1]*length;

	pos[0] = x1;
	pos[1] = x2;


	col[0] = col1[0];
	col[1] = col1[1];
	col[2] = col1[2];


	pixels = g_current_screen_buffer;

	//return;

	for(VCG2DVAR start = 0; start<=1.0; start+=length)
	{
		pos[0] += dirl[0]; //dir[0]*length;
		pos[1] += dirl[1]; //dir[1]*length;


		/// set the pixel
		bufferoffset = (int)((pos[0] + pos[1]*g_current_screen_width)*g_screen_buffer_pixel_size);

		cpixel = &(pixels[bufferoffset]);
		_v = cpixel[3];
		_vd = versionid - _v;

		/// this line sucks !!
		if(_vd >= 1.0)
			_vd = 1.0;
		else continue;

	    cpixel[0] = col[0];
	    cpixel[1] = col[1];
	    cpixel[2] = col[2];

//// r1,g1,b1,t1,i1,si1,bl1, r2,g2,b2,t2,i2,si2,bl2, r3,g3,b3,t3,i3,si3,bl3, vdin, vin, vdout, tall
		///STANDART_COLOR_MIX

	}
}

/// this is alwyas counter clockwise
void draw_line_setversion(VCG2DVAR &x1, VCG2DVAR &y1,
							VCG2DVAR &x2, VCG2DVAR &y2, VERSION_BUFFER_TYPE &version)
{
	dir[0] = x2 - x1;
	dir[1] = y2 - y1;

	/// normalize it
	length = sqrt(1.0/(dir[0]*dir[0]+dir[1]*dir[1])*0.9999999);

	dirl[0] = dir[0]*length;
	dirl[1] = dir[1]*length;
	
	pos[0] = x1;
	pos[1] = y1;

	VERSION_BUFFER_TYPE *pixels_v = g_current_screen_buffer_version;
	bufferoffset = 0;

	bufferoffset = ((int)pos[0] + (int)pos[1]*g_current_screen_width);
	pixels_v[bufferoffset] = version;

	if(dir[1] > 0 && VCG2D::Fill::canWalkXPositiveF(pos[0]))
	{
		bufferoffset = ((int)pos[0]+1 + (int)pos[1]*g_current_screen_width);
		pixels_v[bufferoffset] = version;

	}
	

	for(VCG2DVAR start = 0; start<=1.0; start+=length)
	{

		pos[0] += dirl[0];
		pos[1] += dirl[1];

		bufferoffset = ((int)pos[0] + (int)pos[1]*g_current_screen_width);
		pixels_v[bufferoffset] = version;


			if(dir[1] > 0 && VCG2D::Fill::canWalkXPositiveF(pos[0]))
			{
				bufferoffset = ((int)pos[0]+1 + (int)pos[1]*g_current_screen_width);
				pixels_v[bufferoffset] = version;

			}
	}

	bufferoffset = ((int)x2 + (int)y2*g_current_screen_width);
	pixels_v[bufferoffset] = version;

	if(dir[1] > 0 && VCG2D::Fill::canWalkXPositiveF(x2))
	{
		bufferoffset = ((int)x2+1 + (int)y2*g_current_screen_width);
		pixels_v[bufferoffset] = version;

	}

}



void draw_singleline_onecolor_debug(VCG2DVAR &x1, VCG2DVAR &y1,
							VCG2DVAR &x2, VCG2DVAR &y2, color4_t &color)
{
	dir[0] = x2 - x1;
	dir[1] = y2 - y1;

	/// normalize it
	length = sqrt(1.0/(dir[0]*dir[0]+dir[1]*dir[1])*0.9999999);

	dirl[0] = dir[0]*length;//*0.9999999;
	dirl[1] = dir[1]*length;//*0.9999999;

	pos[0] = x1;
	pos[1] = y1;

	float *pixels_v = g_current_screen_fbuffer;
	bufferoffset = 0;


		bufferoffset = ((int)pos[0] + (int)pos[1]*g_current_screen_width)*g_screen_buffer_pixel_size_internal;

		pixels_v[bufferoffset]   = color.r;
		pixels_v[bufferoffset+1] = color.g;
		pixels_v[bufferoffset+2] = color.b;
		pixels_v[bufferoffset+3] = color.a;


	if(dirl[1] > 0 && VCG2D::Fill::canWalkXPositiveF(x2))
	{
		bufferoffset = ((int)(pos[0]+1) + (int)y1*g_current_screen_width)*g_screen_buffer_pixel_size_internal;
		pixels_v[bufferoffset]   = 1;
		pixels_v[bufferoffset+1] = 0;
		pixels_v[bufferoffset+2] = 0;
		pixels_v[bufferoffset+3] = 0;

	}

	for(VCG2DVAR start = 0; start<=1.0; start+=length)
	{
		pos[0] += dirl[0];
		pos[1] += dirl[1];

		bufferoffset = ((int)pos[0] + (int)pos[1]*g_current_screen_width)*g_screen_buffer_pixel_size_internal;
		pixels_v[bufferoffset]   = color.r;
		pixels_v[bufferoffset+1] = color.g;
		pixels_v[bufferoffset+2] = color.b;
		pixels_v[bufferoffset+3] = color.a;

		if(dirl[1] > 0 && VCG2D::Fill::canWalkXPositiveF(pos[0]))
		{
			bufferoffset+=4;// = ((int)pos[0]+1 + (int)pos[1]*g_current_screen_width);
			pixels_v[bufferoffset]   = 1;
			pixels_v[bufferoffset+1] = 0;
			pixels_v[bufferoffset+2] = 0;
			pixels_v[bufferoffset+3] = 0;

		}


	}

	bufferoffset = ((int)x2 + (int)y2*g_current_screen_width)*g_screen_buffer_pixel_size_internal;
	pixels_v[bufferoffset]   = color.r;
	pixels_v[bufferoffset+1] = color.g;
	pixels_v[bufferoffset+2] = color.b;
	pixels_v[bufferoffset+3] = color.a;

	if(dirl[1] > 0 && VCG2D::Fill::canWalkXPositiveF(x2))
	{
		bufferoffset = ((int)(x2+1) + (int)y2*g_current_screen_width)*g_screen_buffer_pixel_size_internal;
		pixels_v[bufferoffset]   = 1;
		pixels_v[bufferoffset+1] = 0;
		pixels_v[bufferoffset+2] = 0;
		pixels_v[bufferoffset+3] = 0;

	}

}


void draw_singleline_onecolor(VCG2DVAR &x1, VCG2DVAR &y1,
							VCG2DVAR &x2, VCG2DVAR &y2, color4_t &color)
{
	dir[0] = x2 - x1;
	dir[1] = y2 - y1;

	/// normalize it
	length = sqrt(1.0/(dir[0]*dir[0]+dir[1]*dir[1])*0.9999999);

	dirl[0] = dir[0]*length;//*0.9999999;
	dirl[1] = dir[1]*length;//*0.9999999;

	pos[0] = x1;
	pos[1] = y1;

	float *pixels_v = g_current_screen_fbuffer;
	bufferoffset = 0;


		bufferoffset = ((int)pos[0] + (int)pos[1]*g_current_screen_width)*g_screen_buffer_pixel_size_internal;

		pixels_v[bufferoffset]   = color.r;
		pixels_v[bufferoffset+1] = color.g;
		pixels_v[bufferoffset+2] = color.b;
		pixels_v[bufferoffset+3] = color.a;




	for(VCG2DVAR start = 0; start<=1.0; start+=length)
	{
		pos[0] += dirl[0];
		pos[1] += dirl[1];

		bufferoffset = ((int)pos[0] + (int)pos[1]*g_current_screen_width)*g_screen_buffer_pixel_size_internal;
		pixels_v[bufferoffset]   = color.r;
		pixels_v[bufferoffset+1] = color.g;
		pixels_v[bufferoffset+2] = color.b;
		pixels_v[bufferoffset+3] = color.a;




	}

	bufferoffset = ((int)x2 + (int)y2*g_current_screen_width)*g_screen_buffer_pixel_size_internal;
	pixels_v[bufferoffset]   = color.r;
	pixels_v[bufferoffset+1] = color.g;
	pixels_v[bufferoffset+2] = color.b;
	pixels_v[bufferoffset+3] = color.a;


}


}
}
