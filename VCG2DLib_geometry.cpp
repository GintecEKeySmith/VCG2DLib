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




#include "VCG2DLib_geometry.h"
#include "VCG2DLib.h"

namespace VCG2D {

float _fdx = 0;
float _fdy = 0;
float _fdxdy = 0;
float _fdydx = 0;
int _dx = 0;
int _dy = 0;
float _fnewx = 0;
float _fnewy = 0;


/// function to cut a line into a screen window
int prepare_line_for_plotting_firstpoint(VCG2DVAR &x1, VCG2DVAR &y1, 
								VCG2DVAR &x2, VCG2DVAR &y2, bool &fixedone, bool &fixedtwo)
{
/*
 * extern int  g_current_screen_top;
extern int  g_current_screen_bottom;
extern int  g_current_screen_left;
extern int  g_current_screen_right;
 * 
 * */
	if(y1 < g_current_screen_fbottom)
	{
		if(y2 < g_current_screen_fbottom)
			return -1;
			
	}else if(y1 > g_current_screen_ftop)
	{
		if(y2 > g_current_screen_ftop)
			return -1;
	}
	

	if(x1 < g_current_screen_fleft)
	{
		if(x2 < g_current_screen_fleft)
			return -1;
			

		
		
		_dx = x2 - x1;
		
		if(_dx == 0)
			return 0;
			
		
	//	intersection_2lines_02();
		_fdx = g_current_screen_fleft - x1;
		_fdxdy = (float)(y2-y1)/(float)(x2-x1);
		
		_fnewy = _fdxdy * _fdx + y1;
		
		if(_fnewy > g_current_screen_ftop)
		{
			if(y2>y1)
				return 0;
			
			if(x2>g_current_screen_fright and y2>g_current_screen_ftop)
				return 0;
				
			_dy = y2-y1;
			if(_dy == 0)
				return 0;
				
			
			_fdydx = (float)(x2-x1)/(float)(_dy);
			_fnewx = _fdydx * (y1 - g_current_screen_ftop) + x1;
			
			if(_fnewx > g_current_screen_fright)
				return 0;
				
			
			x1 = _fnewx;
			y1 = g_current_screen_ftop;
			
		}else if(_fnewy < g_current_screen_fbottom)
		{
			if(y2<y1)
				return 0;
				
			if(x2>g_current_screen_fright and y2<g_current_screen_fbottom)
				return 0;
				
			_dy = y2-y1;
			if(_dy == 0)
				return 0;
			
			_fdydx = (float)(x2-x1)/(float)(_dy);
			_fnewx = _fdydx * (y1 - g_current_screen_fbottom) + x1;
			
			if(_fnewx > g_current_screen_fright)
				return 0;
				
			x1 = _fnewx;
			y1 = g_current_screen_ftop;
			fixedone = true;
			return 1;
			
		}else
		{
			y1 = _fnewy;
			x1 = g_current_screen_fleft;
			fixedone = true;
			return 1;
		}
		
	}else
	if(x1 <= g_current_screen_fright)
	{


		
		if(x2 < g_current_screen_fleft and y2 > g_current_screen_ftop)
			return 0;
		if(x2 > g_current_screen_fright and y2 > g_current_screen_ftop)
			return 0;
		
	
		if(y1 > g_current_screen_ftop)
		{
			_dy = y2 - y1;
			
			if(_dy == 0)
				return 0;
				
			_fdydx = (float)(x2-x1)/(float)(y2-y1);
			
			_dy = g_current_screen_ftop-y1;
			_fnewx = _dy*_fdydx + x1;
			
			x1 = _fnewx;
			y1 = g_current_screen_ftop;
			fixedone = true;
			return 1;
		}
		else if(y1 == g_current_screen_ftop)
		{
			
			fixedone = false;
			return 1;
			
		}else if(y1 > g_current_screen_fbottom)
		{
			return 1;
			
		}else
		{
			_dy = y2 - y1;
			
			if(_dy == 0)
				return 0;
				
			_fdydx = (float)(x2-x1)/(float)(y2-y1);
			
			_dy = g_current_screen_fbottom - y1;
			_fnewx = _dy*_fdydx + x1;
			
			x1 = _fnewx;
			y1 = g_current_screen_fbottom;
			fixedone = true;
			return 1;
			
		}
		
		
	}else
	{
		
		if(x2 > g_current_screen_fright)
			return -1;
			
		if(y1 < g_current_screen_fbottom)
		{
			if(y2 < g_current_screen_fbottom)
				return 0;
				
		}else if(y1 > g_current_screen_ftop)
		{
			if(y2 > g_current_screen_ftop)
				return 0;
		}
		
		
		_dx = x2 - x1;
		
		if(_dx == 0)
			return 0;
			
		

		_fdx = g_current_screen_fright - x1;
		_fdxdy = (float)(y2-y1)/(float)(x2-x1);
		
		_fnewy = _fdxdy * _fdx + y1;
		
		if(_fnewy > g_current_screen_ftop)
		{
			if(y2>y1)
				return 0;
			
			if(x2 < g_current_screen_fleft and y2 > g_current_screen_ftop)
				return 0;
				
			_dy = y2-y1;
			if(_dy == 0)
				return 0;
				
			
			_fdydx = (float)(x2-x1)/(float)(_dy);
			_fnewx = _fdydx * (y1 - g_current_screen_ftop) + x1;
			
			if(_fnewx < g_current_screen_fleft)
				return 0;
				
			
			x1 = _fnewx;
			y1 = g_current_screen_ftop;
			
		}else if(_fnewy < g_current_screen_fbottom)
		{
			if(y2<y1)
				return 0;
				
			if(x2<g_current_screen_fleft and y2<g_current_screen_fbottom)
				return 0;
				
			_dy = y2-y1;
			if(_dy == 0)
				return 0;
			
			_fdydx = (float)(x2-x1)/(float)(_dy);
			_fnewx = _fdydx * (g_current_screen_fbottom - y1) + x1;
			
			if(_fnewx < g_current_screen_fleft)
				return 0;
				
			x1 = _fnewx;
			y1 = g_current_screen_fbottom;
			fixedone = true;
			return 1;
			
		}else
		{
			y1 = _fnewy;
			x1 = g_current_screen_fright;
			fixedone = true;
			return 1;
		}
		
	}
	
	return 1;
}


/// cuts a line into a screen canves
int prepare_line_for_plotting(VCG2DVAR &x1, VCG2DVAR &y1, 
								VCG2DVAR &x2, VCG2DVAR &y2, bool &fixedone, bool &fixedtwo)
{
	if(prepare_line_for_plotting_firstpoint(x1, y1, x2, y2, fixedone, fixedtwo) == 0)
		return 0;
	
	if(prepare_line_for_plotting_firstpoint(x2, y2, x1, y1, fixedtwo, fixedone) == 0)
		return 0;
	
	return 1;
}


									  
									  




}