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
#include "VCG2DLib_colors.h"
#include "VCG2DLib_fill.h"
#include "VCG2DLib_gradients.h"
#include "VCG2DLib_geometry.h"
#include  <limits>


namespace VCG2D {
namespace Fill {
///pointset where to take points from ....

/// std::vector<points> points;
int _offset = 0;


void fill_xlinepos(int &offset, int &maxiters, VERSION_BUFFER_TYPE &version, color4_t &color)
{
	VCG2DVAR *bufptr = &g_current_screen_fbuffer[offset*VCG2D::g_screen_buffer_pixel_size_internal];

	VERSION_BUFFER_TYPE *bufVversion = &g_current_screen_buffer_version[offset];


	bool stepped = false;


	for(int i = 0; i<maxiters;++i)
	{

		if(bufVversion[0] >= version)
		{

			bufptr[0]  =  color.r;
			bufptr[1]  =  color.g;
			bufptr[2]  =  color.b;
			bufptr[3]  =  color.a;

			if(bufVversion[0] == version)
				bufVversion[0] = version+1;
			else
				bufVversion[0] = version+2;
			

			if(stepped)
				break;
			else
			{
				++bufVversion;
				bufptr += 4;
			
				stepped = true;
			}

	

		}else
		{

			bufptr[0]  =  color.r;
			bufptr[1]  =  color.g;
			bufptr[2]  =  color.b;
			bufptr[3]  =  color.a;

			bufptr += 4;

			if(bufVversion[0] <= version)
				bufVversion[0] = version+1;

			++bufVversion;


			stepped = true;

		}
	}
}


inline void estimate_color_gradient(color4_t &col1,
									color4_t &col2,
									VCG2DVAR &dr,VCG2DVAR &dg,VCG2DVAR &db,VCG2DVAR &da)
{

		dr = col2.r - col1.r;
		dg = col2.g - col2.g;
		db = col2.b - col2.b;
		da = col2.a - col2.a;

}

inline void estimate_color_gradient_from_iter(std::map<float, color4_t>::iterator &col1iter,
									std::map<float, color4_t>::iterator &col2iter,
									VCG2DVAR &dr,VCG2DVAR &dg,VCG2DVAR &db,VCG2DVAR &da)
{


	if(col1iter == col2iter)
	{
		dr = 0;
		dg = 0;
		db = 0;
		da = 0;
		return;
	}

	color4_t &col1 = col1iter->second;
	color4_t &col2 = col2iter->second;

	dr = col2.r - col1.r;
	dg = col2.g - col1.g;
	db = col2.b - col1.b;
	da = col2.a - col1.a;

}


void estimate_first_gradient(
								std::map<float, color4_t>::iterator &currentcolor1,
								std::map<float, color4_t>::iterator &currentcolor2,

								VCG2DVAR &xpos, VCG2DVAR &ypos,

								bool &gradient,

								float &dcgrad,
								float &dcinvgrad,
								float &current_pos,

								float &next_x,

								VCG2D::Gradients::gradient2d_t &colorgrad,

								color4_t &ccolor,
								color4_t &ccolor_grad)
{

	current_pos = VCG2D::Geometry::getProjectedDistancePointLine(
									 colorgrad.m_x1,
									 colorgrad.m_y1,

									 colorgrad.m_x2,
									 colorgrad.m_y2,

									 xpos,
									 ypos);





	if(colorgrad.m_dirx >= 0)
	{

		if(current_pos < 0)
			int stophere = 0;

		colorgrad.getLowerIterator(current_pos, currentcolor1);
		colorgrad.getUpperIterator(current_pos, currentcolor2);


		if(current_pos < currentcolor1->first)
		{

			ccolor.r = currentcolor1->second.r;
			ccolor.g = currentcolor1->second.g;
			ccolor.b = currentcolor1->second.b;
			ccolor.a = currentcolor1->second.a;

			ccolor_grad.r = 0;
			ccolor_grad.g = 0;
			ccolor_grad.b = 0;
			ccolor_grad.a = 0;

			next_x = xpos + current_pos * dcinvgrad * colorgrad.m_length;
			gradient = false;
			return;

		}
		else if( current_pos > currentcolor2->first )
		{
			ccolor.r = currentcolor2->second.r;
			ccolor.g = currentcolor2->second.g;
			ccolor.b = currentcolor2->second.b;
			ccolor.a = currentcolor2->second.a;



			ccolor_grad.r = 0;
			ccolor_grad.g = 0;
			ccolor_grad.b = 0;
			ccolor_grad.a = 0;

			gradient = false;
			next_x = std::numeric_limits<float>::max();
			return;
		}
		else
		{
			float overalllength = currentcolor2->first - currentcolor1->first;

			ccolor_grad.r = (currentcolor2->second.r - currentcolor1->second.r)/colorgrad.m_length;
			ccolor_grad.g = (currentcolor2->second.g - currentcolor1->second.g)/colorgrad.m_length;
			ccolor_grad.b = (currentcolor2->second.b - currentcolor1->second.b)/colorgrad.m_length;
			ccolor_grad.a = (currentcolor2->second.a - currentcolor1->second.a)/colorgrad.m_length;


			float dxgrad = currentcolor2->first - current_pos;



			next_x = xpos + dxgrad * dcinvgrad * colorgrad.m_length;



			float dxgrad2 = (current_pos - currentcolor1->first)/(overalllength);

		   ccolor.r = currentcolor1->second.r + ccolor_grad.r * dxgrad2 * colorgrad.m_length;
		   ccolor.g = currentcolor1->second.g + ccolor_grad.g * dxgrad2 * colorgrad.m_length;			ccolor.b = currentcolor1->second.b + ccolor_grad.b * dxgrad2 * colorgrad.m_length;			 ccolor.a = currentcolor1->second.a + ccolor_grad.a * dxgrad2 * colorgrad.m_length;

			if(colorgrad.m_dirx == 0)
			{
				next_x = std::numeric_limits<float>::max();
				ccolor_grad.r = 0;
				ccolor_grad.g = 0;
				ccolor_grad.b = 0;
				ccolor_grad.a = 0;
				gradient = false;

			}


			return;
		}


	}else
	{
		colorgrad.getLowerIterator(current_pos, currentcolor2);
		colorgrad.getUpperIterator(current_pos, currentcolor1);


		if(current_pos < 0)
			int stophere = 0;


		if( current_pos < currentcolor1->first )
		{

			ccolor.r = currentcolor1->second.r;
			ccolor.g = currentcolor1->second.g;
			ccolor.b = currentcolor1->second.b;
			ccolor.a = currentcolor1->second.a;

			ccolor_grad.r = 0;
			ccolor_grad.g = 0;
			ccolor_grad.b = 0;
			ccolor_grad.a = 0;

			next_x = xpos + current_pos * dcinvgrad * colorgrad.m_length;
			gradient = false;
			return;

		}
		else if( current_pos > currentcolor2->first )
		{
			ccolor.r = currentcolor2->second.r;
			ccolor.g = currentcolor2->second.g;
			ccolor.b = currentcolor2->second.b;
			ccolor.a = currentcolor2->second.a;



			ccolor_grad.r = 0;
			ccolor_grad.g = 0;
			ccolor_grad.b = 0;
			ccolor_grad.a = 0;

			gradient = false;
			next_x = std::numeric_limits<float>::max();
			return;
		}
		else
		{
			float overalllength = currentcolor2->first - currentcolor1->first;

			ccolor_grad.r = (currentcolor2->second.r - currentcolor1->second.r)/colorgrad.m_length;
			ccolor_grad.g = (currentcolor2->second.g - currentcolor1->second.g)/colorgrad.m_length;
			ccolor_grad.b = (currentcolor2->second.b - currentcolor1->second.b)/colorgrad.m_length;
			ccolor_grad.a = (currentcolor2->second.a - currentcolor1->second.a)/colorgrad.m_length;


			float dxgrad = currentcolor2->first - current_pos;



			next_x = xpos + dxgrad * dcinvgrad * colorgrad.m_length;



			float dxgrad2 = (current_pos - currentcolor1->first)/(overalllength);

		   ccolor.r = currentcolor1->second.r + ccolor_grad.r * dxgrad2 * colorgrad.m_length;
		   ccolor.g = currentcolor1->second.g + ccolor_grad.g * dxgrad2 * colorgrad.m_length;			ccolor.b = currentcolor1->second.b + ccolor_grad.b * dxgrad2 * colorgrad.m_length;			 ccolor.a = currentcolor1->second.a + ccolor_grad.a * dxgrad2 * colorgrad.m_length;

			if(colorgrad.m_dirx == 0)
			{
				next_x = std::numeric_limits<float>::max();
				ccolor_grad.r = 0;
				ccolor_grad.g = 0;
				ccolor_grad.b = 0;
				ccolor_grad.a = 0;
				gradient = false;

			}


			return;
		}
	}
}

void estimate_next_gradient(

	std::map<float, color4_t>::iterator &currentcolor1,
	std::map<float, color4_t>::iterator &currentcolor2,

	VCG2DVAR &xpos, VCG2DVAR &ypos,

	float &dcgrad,
	float &dcinvgrad,
	float &current_pos,

	float &next_x,
	bool  &gradient,

	VCG2D::Gradients::gradient2d_t &colorgrad,

	color4_t &ccolor,
	color4_t &ccolor_grad)
{

	if( colorgrad.m_dirx == 0)
	{
		next_x = std::numeric_limits<float>::max();
		gradient = false;
		return;
	}


	if(colorgrad.m_dirx >= 0)
	{
		auto iter_next = currentcolor1;
		++iter_next;


		if(iter_next != colorgrad.m_color_points.end())
		{
			currentcolor1 = iter_next;
			++iter_next;

			if(iter_next == colorgrad.m_color_points.end())
			{
				ccolor.r = currentcolor2->second.r;
				ccolor.g = currentcolor2->second.g;
				ccolor.b = currentcolor2->second.b;
				ccolor.a = currentcolor2->second.a;

				gradient = false;
				next_x = std::numeric_limits<float>::max();

				return;
			}

			ccolor_grad.r = (currentcolor2->second.r - currentcolor1->second.r)*dcgrad;
			ccolor_grad.g = (currentcolor2->second.g - currentcolor1->second.g)*dcgrad;
			ccolor_grad.b = (currentcolor2->second.b - currentcolor1->second.b)*dcgrad;
			ccolor_grad.a = (currentcolor2->second.a - currentcolor1->second.a)*dcgrad;


			current_pos = VCG2D::Geometry::getProjectedDistancePointLine(
									 colorgrad.m_x1,
									 colorgrad.m_y1,

									 colorgrad.m_x2,
									 colorgrad.m_y2,

									 xpos,
									 ypos);

			/// ccolor.r = color_itercurrent->second.r + ();

			float dxgrad = current_pos - currentcolor1->first;

			next_x = xpos + dxgrad * dcinvgrad;

			ccolor.r = currentcolor1->second.r + ccolor_grad.r * dxgrad * dcgrad;
			ccolor.g = currentcolor1->second.g + ccolor_grad.g * dxgrad * dcgrad;
			ccolor.b = currentcolor1->second.b + ccolor_grad.b * dxgrad * dcgrad;			 ccolor.a = currentcolor1->second.a + ccolor_grad.a * dxgrad * dcgrad;
		}else
		{
			next_x = std::numeric_limits<float>::max();
			gradient = false;
			return;
		}

	}else if(colorgrad.m_dirx < 0)
	{

		auto iter_next = currentcolor1;
		++iter_next;


		if(iter_next != colorgrad.m_color_points.end())
		{
			currentcolor1 = iter_next;
			++iter_next;

			if(iter_next == colorgrad.m_color_points.end())
			{
				ccolor.r = currentcolor2->second.r;
				ccolor.g = currentcolor2->second.g;
				ccolor.b = currentcolor2->second.b;
				ccolor.a = currentcolor2->second.a;

				gradient = false;
				next_x = std::numeric_limits<float>::max();

				return;
			}

			ccolor_grad.r = (currentcolor2->second.r - currentcolor1->second.r)*dcgrad;
			ccolor_grad.g = (currentcolor2->second.g - currentcolor1->second.g)*dcgrad;
			ccolor_grad.b = (currentcolor2->second.b - currentcolor1->second.b)*dcgrad;
			ccolor_grad.a = (currentcolor2->second.a - currentcolor1->second.a)*dcgrad;


			current_pos = VCG2D::Geometry::getProjectedDistancePointLine(
									 colorgrad.m_x1,
									 colorgrad.m_y1,

									 colorgrad.m_x2,
									 colorgrad.m_y2,

									 xpos,
									 ypos);

			/// ccolor.r = color_itercurrent->second.r + ();

			float dxgrad = current_pos - currentcolor1->first;

			next_x = xpos + dxgrad * dcinvgrad;

			ccolor.r = currentcolor1->second.r + ccolor_grad.r * dxgrad * dcgrad;
			ccolor.g = currentcolor1->second.g + ccolor_grad.g * dxgrad * dcgrad;
			ccolor.b = currentcolor1->second.b + ccolor_grad.b * dxgrad * dcgrad;			 ccolor.a = currentcolor1->second.a + ccolor_grad.a * dxgrad * dcgrad;
		}else
		{
			next_x = std::numeric_limits<float>::max();
			gradient = false;
			return;
		}

	}

}


void fill_xlinepos_gradient(int &offset, int &maxiters,
								VCG2DVAR &xpos, VCG2DVAR &ypos,
								VERSION_BUFFER_TYPE &version,

								VCG2D::Gradients::gradient2d_t &colorgrad)
{

	VCG2DVAR *bufptr = &g_current_screen_fbuffer[offset*VCG2D::g_screen_buffer_pixel_size_internal];

	VERSION_BUFFER_TYPE *bufVversion = &g_current_screen_buffer_version[offset];
	bool stepped = false;
	bool fgradient = true;


	//float dcgrad = colorgrad.estimteGradientProjectionOntoXAxis();
	float currentposx;
	float next_x;

	color4_t ccolor;
	color4_t ccolor_grad;

	std::map<float, color4_t>::iterator currentcolor1;
	std::map<float, color4_t>::iterator currentcolor2;

	estimate_first_gradient(

		currentcolor1,
		currentcolor2,

		xpos, ypos,

		fgradient,

		colorgrad.m_x_axis_projection_grad,
		colorgrad.m_x_axis_projection_grad_inv,

		currentposx,
		next_x,

		colorgrad,

		ccolor,
		ccolor_grad);

	float cxpos = xpos;

	for(int i = 0; i<maxiters;++i)
	{

		if(bufVversion[0] >= version)
		{

			bufptr[0]  = ccolor.r;
			bufptr[1]  = ccolor.g;
			bufptr[2]  = ccolor.b;
			bufptr[3]  = ccolor.a;

			if(bufVversion[0] == version)
				bufVversion[0] = version+1;
			else
				bufVversion[0] = version+2;
			//bufVversion[0] = version+2;

			if(stepped)
				break;
			else
			{
				++bufVversion;
				//++counter;
				bufptr += 4;
				//ptrcounter += 4;
				stepped = true;
			}

			//break;

		}else
		{
            stepped = true;

			bufptr[0]  =  ccolor.r;
			bufptr[1]  =  ccolor.g;
			bufptr[2]  =  ccolor.b;
			bufptr[3]  =  ccolor.a;

			bufptr += 4;

			if(bufVversion[0] <= version)
				bufVversion[0] = version+1;

			++bufVversion;


			//stepped = true;

		}

		cxpos += 1.0;

		if(fgradient)
		{
			ccolor.r += ccolor_grad.r*colorgrad.m_x_axis_projection_grad;
			ccolor.g += ccolor_grad.g*colorgrad.m_x_axis_projection_grad;
			ccolor.b += ccolor_grad.b*colorgrad.m_x_axis_projection_grad;
			ccolor.a += ccolor_grad.a*colorgrad.m_x_axis_projection_grad;
		}

		if(cxpos > next_x)
		{
			/// estimate the next
			estimate_next_gradient(

				currentcolor1,
				currentcolor2,

				cxpos, ypos,

				colorgrad.m_x_axis_projection_grad,
				colorgrad.m_x_axis_projection_grad_inv,

				currentposx,
				next_x,

				fgradient,

				colorgrad,

				ccolor,
				ccolor_grad  );
		}
	}
}


}
}
