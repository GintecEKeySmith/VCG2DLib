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


#ifndef __VCG2DLIB_FILL_H
#define __VCG2DLIB_FILL_H


#include "VCG2DLib_prec.h"
#include "VCG2DLib_gradients.h"

namespace VCG2D {
namespace Fill {

void fill_area(int &xstart, int &ystart, VERSION_BUFFER_TYPE &firstversion,
					VERSION_BUFFER_TYPE &lastversion,
					VCG2D::color4_t &col1);
					
void fill_line_onecolor(VCG2DVAR &x1, VCG2DVAR &y1, 
							VCG2DVAR &x2, VCG2DVAR &y2, 
								color4_t &color, 
									VERSION_BUFFER_TYPE &versionid);

void fill_xlinepos(int &offset, int &maxiters, VERSION_BUFFER_TYPE &version, color4_t &color);	

void fill_line_onecolor_gradient(
							VCG2DVAR &x1, VCG2DVAR &y1,
							VCG2DVAR &x2, VCG2DVAR &y2,
							Gradients::gradient2d_t &colorgrad,
							VERSION_BUFFER_TYPE &version
							);
								
void fill_xlinepos_gradient(int &offset, int &maxiters,
								VCG2DVAR &xpos, VCG2DVAR &ypos,
								VERSION_BUFFER_TYPE &version, 
								
								VCG2D::Gradients::gradient2d_t &colorgrad);

inline bool canWalkXPositive(int &x_pos)
{
	if( x_pos + 1 > g_current_screen_right )
	{
		return false;
	}
	
	return true;
}

inline bool canWalkXNegative(int &x_pos)
{
	
	if( x_pos - 1 < g_current_screen_left )
	{
		return false;
	}

	return true;
}

inline bool canWalkYPositive(int &y_pos)
{

	if( y_pos + 1 > g_current_screen_top )
	{
		return false;
	}

	return true;
	
}

inline bool canWalkYNegative(int &y_pos)
{

	if( y_pos - 1 < g_current_screen_bottom )
	{
		return false;
	}
	
	return true;	
}	
	
inline bool canWalkXPositiveF(VCG2DVAR &x_pos)
{
	if( x_pos + 1.0 > g_current_screen_fright )
	{
		return false;
	}
	
	return true;
}

inline bool canWalkXNegativeF(VCG2DVAR &x_pos)
{
	
	if( x_pos - 1.0 < g_current_screen_fleft )
	{
		return false;
	}

	return true;
}

inline bool canWalkYPositive(VCG2DVAR &y_pos)
{

	if( y_pos + 1.0 > g_current_screen_ftop )
	{
		return false;
	}

	return true;
	
}

inline bool canWalkYNegative(VCG2DVAR &y_pos)
{

	if( y_pos - 1.0 < g_current_screen_fbottom )
	{
		return false;
	}
	
	return true;	
}		
	
}}
#endif