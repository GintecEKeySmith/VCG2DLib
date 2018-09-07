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




#include "VCG2DLib_gradients.h"
#include "VCG2DLib_geometry.h"

namespace VCG2D{
namespace Gradients{

float gradient2d_t::estimteGradientProjectionOntoXAxis()
{

	return sqrt(1 + m_diry*m_diry/(m_dirx*m_dirx)); 
	
}

void  gradient2d_t::estimateXAxisTransformation()
{
	
	m_x_axis_projection_grad = estimteGradientProjectionOntoXAxis();
	m_x_axis_projection_grad_inv = 1.0/m_x_axis_projection_grad;
	
}

void gradient2d_t::estimateLength()
{
	m_length = VCG2D::Geometry::VectorLength(m_x1, m_x2, m_y1, m_y2);
}

bool gradient2d_t::getLowerIterator(float &value, 
									std::map<float, color4_t>::iterator &iter)
{
	
	auto iter1 = m_color_points.lower_bound(value);
	
	if(iter1->first > value and iter1 != m_color_points.begin())
	{
		iter = iter1;
		--iter;
	}else if(iter1 == m_color_points.end())
	{
		iter = m_color_points.end();
		--iter;
	}
	else iter = iter1;
	
	return true;
}


bool gradient2d_t::getUpperIterator(float &value, 
									std::map<float, color4_t>::iterator &iter)
{
	
	auto iter1 = m_color_points.lower_bound(value);
	
	if(iter1->first < value and iter1 != m_color_points.end())
	{
		++iter1;
		
		if(m_color_points.end() == iter1)
			--iter1;
	}else if(m_color_points.end() == iter1)
	{
		
		iter1 = m_color_points.end();
		--iter1;
		
	}else
	{
		
	}
	
	iter = iter1;
	
	return true;
}

	
float gradient2d_t::getNextProjectedPosOnHorizontalLine(

														VCG2DVAR &ycurrent_nextpos,
														VCG2DVAR &xcur_next,
														
														std::map<float, color4_t>::iterator &currentiter,
 
														VCG2D::color4_t &colorout_beg,
														VCG2D::color4_t &colorout_end,
														
														bool &no_gradient)
{
	
	//currentiter = ;
	
	auto iter1 = m_color_points.lower_bound(ycurrent_nextpos);
	auto iter2 = m_color_points.upper_bound(ycurrent_nextpos);
	
	if(iter1 == iter2)
	{ 
		no_gradient = true;
		colorout_beg = iter1->second;
		return 1;
	}
	
}


void gradient2d_t::estimateDir()
{
	m_dirx = m_x2 - m_x1;
	m_diry = m_y2 - m_y1;
}



}
}