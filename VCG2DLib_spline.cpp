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
#include "math.h"

namespace VCG2D {
namespace Splines{

VCG2DVAR _tmp;
VCG2DVAR _tmp2;
//CG2DVAR _t;
VCG2DVAR _tmp_x;
VCG2DVAR _tmp_y;

VCG2DVAR _length;
VCG2DVAR _start;
//CG2DVAR _;
int _offset;
bool _result = false;

inline void setColorTo(VCG2DVAR *ptr, color4_t &col)
{
	ptr[0] = col.r;
	ptr[1] = col.g;
	ptr[2] = col.b;
	ptr[3] = col.a;
}

inline void quadratic_bazier_curve(VCG2DVAR &x0, VCG2DVAR &y0,
									VCG2DVAR &x1, VCG2DVAR &y1,
									VCG2DVAR &x2, VCG2DVAR &y2,
									VCG2DVAR &t,
									VCG2DVAR &xres, VCG2DVAR &yres )
{
    _tmp = (1.0-t);
	xres = (_tmp)*((_tmp)*x0+t*x1)+t*((_tmp)*x1+t*x2);
	yres = (_tmp)*((_tmp)*y0+t*y1)+t*((_tmp)*y1+t*y2);
}

inline void quadratic_bazier_curve_div1(VCG2DVAR &x0, VCG2DVAR &y0,
										VCG2DVAR &x1, VCG2DVAR &y1,
										VCG2DVAR &x2, VCG2DVAR &y2, VCG2DVAR &t,
											VCG2DVAR &xres, VCG2DVAR &yres)

{

    _tmp = (1.0-t);
	xres = 2.0*_tmp*(x1-x0)+2.0*t*(x2-x1);
	yres = 2.0*_tmp*(y1-y0)+2.0*t*(y2-y1);

}

inline void quadratic_bazier_curve_div2(VCG2DVAR &x0, VCG2DVAR &y0,
										VCG2DVAR &x1, VCG2DVAR &y1,
										VCG2DVAR &x2, VCG2DVAR &y2,
										VCG2DVAR &t,
										VCG2DVAR &xres, VCG2DVAR &yres)
{
	xres = 2.0*(x2-2.0*x1+x0);
	yres = 2.0*(y2-2.0*y1+y0);
}


inline void cubic_bazier_curve(VCG2DVAR &x0, VCG2DVAR &y0,
							VCG2DVAR &x1, VCG2DVAR &y1,
								VCG2DVAR &x2, VCG2DVAR &y2,
									VCG2DVAR &x3, VCG2DVAR &y3,
										VCG2DVAR &t,
											VCG2DVAR &xres, VCG2DVAR &yres)
{
    _tmp = (1-t);
    _tmp2 = _tmp*_tmp;

	xres = (_tmp2)*_tmp*x0+3*_tmp2*t*x1+3*(_tmp)*t*t*x2+t*t*t*x3;
	yres = (_tmp2)*_tmp*y0+3*_tmp2*t*y1+3*(_tmp)*t*t*y2+t*t*t*y3;
}

inline void cubic_bazier_curve_div1(VCG2DVAR &x0, VCG2DVAR &y0,
							VCG2DVAR &x1, VCG2DVAR &y1,
								VCG2DVAR &x2, VCG2DVAR &y2,
									VCG2DVAR &x3, VCG2DVAR &y3,
										VCG2DVAR &t,
											VCG2DVAR &xres, VCG2DVAR &yres)
{

    _tmp = (1-t);
    _tmp2 = _tmp*_tmp;

	xres = 3*(_tmp2)*(x1-x0)+6*(_tmp)*t*(x2-x1)+3*t*t*(x3-x2);
	yres = 3*(_tmp2)*(y1-y0)+6*(_tmp)*t*(y2-y1)+3*t*t*(y3-y2);

}

inline void cubic_bazier_curve_div2(VCG2DVAR &x0, VCG2DVAR &y0,
							VCG2DVAR &x1, VCG2DVAR &y1,
								VCG2DVAR &x2, VCG2DVAR &y2,
									VCG2DVAR &x3, VCG2DVAR &y3,
										VCG2DVAR &t,
											VCG2DVAR &xres, VCG2DVAR &yres)
{
    _tmp = (1.0-t);
    _tmp2 = _tmp*_tmp;

	xres = 6*(_tmp)*(x2-2.0*x1+x0)+6.0*t*(x3-2.0*x2+x1);
	yres = 6*(_tmp)*(y2-2.0*y1+y0)+6.0*t*(y3-2.0*y2+y1);
}


inline bool point_is_in_quader( VCG2DVAR &x1, VCG2DVAR &y1,
				VCG2DVAR &left,
					VCG2DVAR &right,
						VCG2DVAR &bottom,
							VCG2DVAR &top)
{

	if(x1 < left)
		return false;
	if(x1 > right)
		return false;
	if(y1 < bottom)
		return false;
	if(y1 > top)
		return false;

	return true;
}

inline bool point_is_inscreen( VCG2DVAR &x1, VCG2DVAR &y1 )
{

	return point_is_in_quader(x1, y1,
								g_current_screen_fleft,
									g_current_screen_fright,
										g_current_screen_fbottom,
											g_current_screen_ftop);

}



void draw_quadratic_bazier_line(VCG2DVAR &x0, VCG2DVAR &y0,
									VCG2DVAR &x1, VCG2DVAR &y1,
										VCG2DVAR &x2, VCG2DVAR &y2,
												VERSION_BUFFER_TYPE &versionnumber )
{


	VERSION_BUFFER_TYPE *pixels_v = g_current_screen_buffer_version;
	_offset = 0;


	for( _start = 0; _start<=1.0; _start+=_length)
	{

		quadratic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, _start, _tmp_x, _tmp_y );
		_length = sqrt(1.0/(_tmp_x*_tmp_x+_tmp_y*_tmp_y)*0.9999999);

		if(!point_is_in_quader(_tmp_x,_tmp_y,
						g_current_screen_fleft,
						g_current_screen_fright,
						g_current_screen_fbottom,
						g_current_screen_ftop))
		{
			continue;
		}

		_offset = (_tmp_x + _tmp_y*g_current_screen_width);
		pixels_v[_offset] = versionnumber;

	}

}


void draw_cubic_bazier_line(VCG2DVAR &x0, VCG2DVAR &y0,
									VCG2DVAR &x1, VCG2DVAR &y1,
										VCG2DVAR &x2, VCG2DVAR &y2,
											VCG2DVAR &x3, VCG2DVAR &y3,
												VERSION_BUFFER_TYPE &versionnumber)
{

	VERSION_BUFFER_TYPE *pixels_v = g_current_screen_buffer_version;
	_offset = 0;



	for( _start = 0; _start<=1.0; _start+=_length)
	{

		cubic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, x3, y3, _start, _tmp_x, _tmp_y );
		_length = sqrt(1.0/(_tmp_x*_tmp_x+_tmp_y*_tmp_y)*0.9999999);

		if(!point_is_in_quader(_tmp_x,_tmp_y,
						g_current_screen_fleft,
						g_current_screen_fright,
						g_current_screen_fbottom,
						g_current_screen_ftop))
		{
			continue;
		}

		_offset = (_tmp_x + _tmp_y*g_current_screen_width);
		pixels_v[_offset] = versionnumber;

	}

}

/***
 * * Drawing functions
 * *
 * *
 * **/

void draw_quadratic_bazier_line_setversion(VCG2DVAR &x0, VCG2DVAR &y0,
									VCG2DVAR &x1, VCG2DVAR &y1,
										VCG2DVAR &x2, VCG2DVAR &y2,
												VERSION_BUFFER_TYPE &version)
{

	VERSION_BUFFER_TYPE *pixels_v = g_current_screen_buffer_version;

	//float *pixels_v = g_current_screen_fbuffer;
	int bufferoffset = 0;

	bufferoffset = ((int)x1 + (int)y1*g_current_screen_width);

	if(point_is_in_quader(x1, y1, g_current_screen_fleft,
									g_current_screen_fright,
										g_current_screen_fbottom,
											g_current_screen_ftop ))
												pixels_v[bufferoffset] = version;


	/// one two three four five six

	_start = 0.0;
	quadratic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, _start, _tmp_x, _tmp_y );
	_length = (1.0/sqrt(_tmp_x*_tmp_x+_tmp_y*_tmp_y)*0.9999999);

	VCG2DVAR pos[2];



	if(_tmp_y > 0 && VCG2D::Fill::canWalkXPositiveF( x1 ))
	{

		bufferoffset = ((int)x1+1 + (int)y1*g_current_screen_width);
		pixels_v[bufferoffset] = version;

		//setColorTo(&pixels_v[bufferoffset], color);

	}

	for( _start = 0; _start<=1.0; )
	{
		quadratic_bazier_curve( x0, y0, x1, y1, x2, y2, _start, pos[0], pos[1]);
		quadratic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, _start, _tmp_x, _tmp_y );
		_length = (1.0/sqrt(_tmp_x*_tmp_x+_tmp_y*_tmp_y)*0.9999999);

		bufferoffset = ((int)pos[0] + (int)pos[1]*g_current_screen_width);

		if(point_is_in_quader(pos[0], pos[1], g_current_screen_fleft,
									g_current_screen_fright,
										g_current_screen_fbottom,
											g_current_screen_ftop ))
											{

					pixels_v[bufferoffset] = version;



					if(_tmp_y > 0 && VCG2D::Fill::canWalkXPositiveF(pos[0]))
					{

							bufferoffset = ((int)pos[0]+1 + (int)pos[1]*g_current_screen_width);
							//setColorTo(&pixels_v[bufferoffset], color);
							pixels_v[bufferoffset] = version;

					}

		}

		_start += _length;
	}


	bufferoffset = ((int)x2 + (int)y2*g_current_screen_width);

	if(point_is_in_quader(x2, y2, g_current_screen_fleft,
						g_current_screen_fright,
							g_current_screen_fbottom,
								g_current_screen_ftop ));
								{
									pixels_v[bufferoffset] = version;


			_start = 1.0;
			quadratic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, _start, _tmp_x, _tmp_y );

			if(_tmp_y > 0 && VCG2D::Fill::canWalkXPositiveF(x2))
			{

				bufferoffset = ((int)x2+1 + (int)y2*g_current_screen_width);
				pixels_v[bufferoffset] = version;

			}
	}
}


void draw_quadratic_bazier_line_onecolor_debug(VCG2DVAR &x0, VCG2DVAR &y0,
											VCG2DVAR &x1, VCG2DVAR &y1,
												VCG2DVAR &x2, VCG2DVAR &y2,
													VCG2D::color4_t &color)
{
	float *pixels_v = g_current_screen_fbuffer;
	int bufferoffset = 0;

	bufferoffset = ((int)x1 + (int)y1*g_current_screen_width)*g_screen_buffer_pixel_size_internal;

	if(point_is_in_quader(x1, y1, g_current_screen_fleft,
									g_current_screen_fright,
										g_current_screen_fbottom,
											g_current_screen_ftop ))
												setColorTo(&pixels_v[bufferoffset], color);


	/// one two three four five six

	_start = 0.0;
	quadratic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, _start, _tmp_x, _tmp_y );
	_length = (1.0/sqrt(_tmp_x*_tmp_x+_tmp_y*_tmp_y)*0.9999999);

	VCG2DVAR pos[2];



	if(_tmp_y > 0 && VCG2D::Fill::canWalkXPositiveF( x1 ))
	{

		bufferoffset = ((int)x1+1 + (int)y1*g_current_screen_width)*g_screen_buffer_pixel_size_internal;
		setColorTo(&pixels_v[bufferoffset], color);

	}

	for( _start = 0; _start<=1.0; )
	{
		quadratic_bazier_curve( x0, y0, x1, y1, x2, y2, _start, pos[0], pos[1]);
		quadratic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, _start, _tmp_x, _tmp_y );
		_length = (1.0/sqrt(_tmp_x*_tmp_x+_tmp_y*_tmp_y)*0.9999999);

		bufferoffset = ((int)pos[0] + (int)pos[1]*g_current_screen_width)*g_screen_buffer_pixel_size_internal;

		if(point_is_in_quader(pos[0], pos[1], g_current_screen_fleft,
									g_current_screen_fright,
										g_current_screen_fbottom,
											g_current_screen_ftop ))
											{

					setColorTo(&pixels_v[bufferoffset], color);



					if(_tmp_y > 0 && VCG2D::Fill::canWalkXPositiveF(pos[0]))
					{
							bufferoffset = ((int)pos[0]+1 + (int)pos[1]*g_current_screen_width)*g_screen_buffer_pixel_size_internal;
							//setColorTo(&pixels_v[bufferoffset], color);
							setColorTo(&pixels_v[bufferoffset], color);

					}

		}

		_start += _length;
	}


	bufferoffset = ((int)x2 + (int)y2*g_current_screen_width)*g_screen_buffer_pixel_size_internal;

	if(point_is_in_quader(x2, y2, g_current_screen_fleft,
						g_current_screen_fright,
							g_current_screen_fbottom,
								g_current_screen_ftop ));
								{
									setColorTo(&pixels_v[bufferoffset], color);
								}

	_start = 1.0;
	quadratic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, _start, _tmp_x, _tmp_y );

	if(_tmp_y > 0 && VCG2D::Fill::canWalkXPositiveF(x2))
	{

		bufferoffset = ((int)x2+1 + (int)y2*g_current_screen_width)*g_screen_buffer_pixel_size_internal;
		setColorTo(&pixels_v[bufferoffset], color);

	}

}

void draw_quadratic_bazier_line_onecolor(VCG2DVAR &x0, VCG2DVAR &y0,
											VCG2DVAR &x1, VCG2DVAR &y1,
												VCG2DVAR &x2, VCG2DVAR &y2,
													VCG2D::color4_t &color)
{
	float *pixels_v = g_current_screen_fbuffer;
	int bufferoffset = 0;

	bufferoffset = ((int)x1 + (int)y1*g_current_screen_width)*g_screen_buffer_pixel_size_internal;

	if(point_is_in_quader(x1, y1, g_current_screen_fleft,
									g_current_screen_fright,
										g_current_screen_fbottom,
											g_current_screen_ftop ))
												setColorTo(&pixels_v[bufferoffset], color);


	/// one two three four five six

	_start = 0.0;
	quadratic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, _start, _tmp_x, _tmp_y );
	_length = (1.0/sqrt(_tmp_x*_tmp_x+_tmp_y*_tmp_y)*0.9999999);

	VCG2DVAR pos[2];



	for( _start = 0; _start<=1.0; )
	{
		quadratic_bazier_curve( x0, y0, x1, y1, x2, y2, _start, pos[0], pos[1]);
		quadratic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, _start, _tmp_x, _tmp_y );
		_length = (1.0/sqrt(_tmp_x*_tmp_x+_tmp_y*_tmp_y)*0.9999999);

		bufferoffset = ((int)pos[0] + (int)pos[1]*g_current_screen_width)*g_screen_buffer_pixel_size_internal;

		if(point_is_in_quader(pos[0], pos[1], g_current_screen_fleft,
									g_current_screen_fright,
										g_current_screen_fbottom,
											g_current_screen_ftop ))
											{

					setColorTo(&pixels_v[bufferoffset], color);



		}

		_start += _length;
	}


	bufferoffset = ((int)x2 + (int)y2*g_current_screen_width)*g_screen_buffer_pixel_size_internal;

	if(point_is_in_quader(x2, y2, g_current_screen_fleft,
						g_current_screen_fright,
							g_current_screen_fbottom,
								g_current_screen_ftop ));
								{
									setColorTo(&pixels_v[bufferoffset], color);
								}

	_start = 1.0;
	quadratic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, _start, _tmp_x, _tmp_y );


}


void draw_cubic_bazier_line_setversion(VCG2DVAR &x0, VCG2DVAR &y0,
										VCG2DVAR &x1, VCG2DVAR &y1,
										VCG2DVAR &x2, VCG2DVAR &y2,
										VCG2DVAR &x3, VCG2DVAR &y3,
											VERSION_BUFFER_TYPE &version)
{

	VERSION_BUFFER_TYPE *pixels_v = g_current_screen_buffer_version;

	//float *pixels_v = g_current_screen_fbuffer;
	int bufferoffset = 0;

	bufferoffset = ((int)x1 + (int)y1*g_current_screen_width);

	if(point_is_in_quader(x1, y1, g_current_screen_fleft,
									g_current_screen_fright,
										g_current_screen_fbottom,
											g_current_screen_ftop ))
												pixels_v[bufferoffset] = version;


	/// one two three four five six

	_start = 0.0;
	cubic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, x3, y3, _start, _tmp_x, _tmp_y );
	_length = (1.0/sqrt(_tmp_x*_tmp_x+_tmp_y*_tmp_y)*0.9999999);

	VCG2DVAR pos[2];



	if(_tmp_y > 0 && VCG2D::Fill::canWalkXPositiveF( x1 ))
	{

		bufferoffset = ((int)x1+1 + (int)y1*g_current_screen_width);
		pixels_v[bufferoffset] = version;

		//setColorTo(&pixels_v[bufferoffset], color);

	}

	for( _start = 0; _start<=1.0; )
	{
		cubic_bazier_curve( x0, y0, x1, y1, x2, y2, x3, y3, _start, pos[0], pos[1]);
		cubic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, x3, y3, _start, _tmp_x, _tmp_y );
		_length = (1.0/sqrt(_tmp_x*_tmp_x+_tmp_y*_tmp_y)*0.9999999);

		bufferoffset = ((int)pos[0] + (int)pos[1]*g_current_screen_width);

		if(point_is_in_quader(pos[0], pos[1], g_current_screen_fleft,
									g_current_screen_fright,
										g_current_screen_fbottom,
											g_current_screen_ftop ))
		{

					pixels_v[bufferoffset] = version;



					if(_tmp_y > 0 && VCG2D::Fill::canWalkXPositiveF(pos[0]))
					{

							bufferoffset = ((int)pos[0]+1 + (int)pos[1]*g_current_screen_width);
							//setColorTo(&pixels_v[bufferoffset], color);
							pixels_v[bufferoffset] = version;

					}

		}

		_start += _length;
	}


	bufferoffset = ((int)x3 + (int)y3*g_current_screen_width);

	if(point_is_in_quader(x3, y3, g_current_screen_fleft,
						g_current_screen_fright,
							g_current_screen_fbottom,
								g_current_screen_ftop ));
								{
									pixels_v[bufferoffset] = version;


			_start = 1.0;
			cubic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, x3, y3, _start, _tmp_x, _tmp_y );

			if(_tmp_y > 0 && VCG2D::Fill::canWalkXPositiveF(x3))
			{

				bufferoffset = ((int)x3+1 + (int)y3*g_current_screen_width);
				pixels_v[bufferoffset] = version;

			}
	}
}


void draw_cubic_bazier_line_onecolor_debug(VCG2DVAR &x0, VCG2DVAR &y0,
											VCG2DVAR &x1, VCG2DVAR &y1,
												VCG2DVAR &x2, VCG2DVAR &y2,
												  VCG2DVAR &x3, VCG2DVAR &y3,

													VCG2D::color4_t &color)
{
	float *pixels_v = g_current_screen_fbuffer;
	int bufferoffset = 0;

	bufferoffset = ((int)x1 + (int)y1*g_current_screen_width)*g_screen_buffer_pixel_size_internal;

	if(point_is_in_quader(x1, y1, g_current_screen_fleft,
									g_current_screen_fright,
										g_current_screen_fbottom,
											g_current_screen_ftop ))
												setColorTo(&pixels_v[bufferoffset], color);


	/// one two three four five six

	_start = 0.0;
	cubic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, x3, y3, _start, _tmp_x, _tmp_y );
	_length = (1.0/sqrt(_tmp_x*_tmp_x+_tmp_y*_tmp_y)*0.9999999);

	VCG2DVAR pos[2];



	if(_tmp_y > 0 && VCG2D::Fill::canWalkXPositiveF( x1 ))
	{

		bufferoffset = ((int)x1+1 + (int)y1*g_current_screen_width)*g_screen_buffer_pixel_size_internal;
		setColorTo(&pixels_v[bufferoffset], color);

	}

	for( _start = 0; _start<=1.0; )
	{
		cubic_bazier_curve( x0, y0, x1, y1, x2, y2, x3, y3, _start, pos[0], pos[1]);
		cubic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, x3, y3, _start, _tmp_x, _tmp_y );
		_length = (1.0/sqrt(_tmp_x*_tmp_x+_tmp_y*_tmp_y)*0.9999999);

		bufferoffset = ((int)pos[0] + (int)pos[1]*g_current_screen_width)*g_screen_buffer_pixel_size_internal;

		if(point_is_in_quader(pos[0], pos[1], g_current_screen_fleft,
									g_current_screen_fright,
										g_current_screen_fbottom,
											g_current_screen_ftop ))
											{

					setColorTo(&pixels_v[bufferoffset], color);



					if(_tmp_y > 0 && VCG2D::Fill::canWalkXPositiveF(pos[0]))
					{
							bufferoffset = ((int)pos[0]+1 + (int)pos[1]*g_current_screen_width)*g_screen_buffer_pixel_size_internal;
							//setColorTo(&pixels_v[bufferoffset], color);
							setColorTo(&pixels_v[bufferoffset], color);

					}

		}

		_start += _length;
	}


	bufferoffset = ((int)x3 + (int)y3*g_current_screen_width)*g_screen_buffer_pixel_size_internal;

	if(point_is_in_quader(x3, y3, g_current_screen_fleft,
						g_current_screen_fright,
							g_current_screen_fbottom,
								g_current_screen_ftop ));
								{
									setColorTo(&pixels_v[bufferoffset], color);
								}

	_start = 1.0;
	cubic_bazier_curve_div1( x0, y0,
					x1, y1,
					x2, y2, x3, y3,
					_start, _tmp_x, _tmp_y );

	if(_tmp_y > 0 && VCG2D::Fill::canWalkXPositiveF(x3))
	{

		bufferoffset = ((int)x3+1 + (int)y3*g_current_screen_width)*g_screen_buffer_pixel_size_internal;
		setColorTo(&pixels_v[bufferoffset], color);

	}

}

void draw_cubic_bazier_line_onecolor(VCG2DVAR &x0, VCG2DVAR &y0,
											VCG2DVAR &x1, VCG2DVAR &y1,
												VCG2DVAR &x2, VCG2DVAR &y2,
												  VCG2DVAR &x3, VCG2DVAR &y3,

													VCG2D::color4_t &color)
{
	float *pixels_v = g_current_screen_fbuffer;
	int bufferoffset = 0;

	bufferoffset = ((int)x1 + (int)y1*g_current_screen_width)*g_screen_buffer_pixel_size_internal;

	if(point_is_in_quader(x1, y1, g_current_screen_fleft,
									g_current_screen_fright,
										g_current_screen_fbottom,
											g_current_screen_ftop ))
												setColorTo(&pixels_v[bufferoffset], color);


	/// one two three four five six

	_start = 0.0;
	cubic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, x3, y3, _start, _tmp_x, _tmp_y );
	_length = (1.0/sqrt(_tmp_x*_tmp_x+_tmp_y*_tmp_y)*0.9999999);

	VCG2DVAR pos[2];




	for( _start = 0; _start<=1.0; )
	{
		cubic_bazier_curve( x0, y0, x1, y1, x2, y2, x3, y3, _start, pos[0], pos[1]);
		cubic_bazier_curve_div1( x0, y0, x1, y1, x2, y2, x3, y3, _start, _tmp_x, _tmp_y );
		_length = (1.0/sqrt(_tmp_x*_tmp_x+_tmp_y*_tmp_y)*0.9999999);

		bufferoffset = ((int)pos[0] + (int)pos[1]*g_current_screen_width)*g_screen_buffer_pixel_size_internal;

		if(point_is_in_quader(pos[0], pos[1], g_current_screen_fleft,
									g_current_screen_fright,
										g_current_screen_fbottom,
											g_current_screen_ftop ))
											{

					setColorTo(&pixels_v[bufferoffset], color);



		}

		_start += _length;
	}


	bufferoffset = ((int)x3 + (int)y3*g_current_screen_width)*g_screen_buffer_pixel_size_internal;

	if(point_is_in_quader(x3, y3, g_current_screen_fleft,
						g_current_screen_fright,
							g_current_screen_fbottom,
								g_current_screen_ftop ));
								{
									setColorTo(&pixels_v[bufferoffset], color);
								}

	_start = 1.0;
	cubic_bazier_curve_div1( x0, y0,
					x1, y1,
					x2, y2, x3, y3,
					_start, _tmp_x, _tmp_y );

}


}
}
