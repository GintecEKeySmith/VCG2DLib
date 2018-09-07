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




#ifndef __VCG2DLIB_GRADIENT_H
#define __VCG2DLIB_GRADIENT_H

#include "VCG2DLib_prec.h"
#include "VCG2DLib_colors.h"
#include <map>

namespace VCG2D {
namespace Gradients {	


struct gradient2d_t
{
	VCG2DVAR m_x1;
	VCG2DVAR m_y1;
	VCG2DVAR m_x2;
	VCG2DVAR m_y2;
	
	VCG2DVAR m_length;
	
	/// direction from x1 to x2
	VCG2DVAR m_dirx;
	/// direction from y1 to y2
	VCG2DVAR m_diry;
	
	/// projection between xAxis and direction vector (Cartesian coordinates)
	VCG2DVAR m_x_axis_projection_grad; 
	VCG2DVAR m_x_axis_projection_grad_inv;
	
	void estimateDir();
	
	float getNextProjectedPosOnHorizontalLine(VCG2DVAR &ycurrent_nextpos,
														VCG2DVAR &xcur_next,
														
														std::map<float, color4_t>::iterator &currentiter,
 
														VCG2D::color4_t &colorout_beg,
														VCG2D::color4_t &colorout_end,
														
														bool &no_gradient);
	/// transformation coefficient between xAxis and direction vector													
	float estimteGradientProjectionOntoXAxis();
	/// updates the gradients xaxis direction projection variables
	void estimateXAxisTransformation();
	/// updates the length variable of the gradient
	void estimateLength();
	
	bool getLowerIterator(float &value, std::map<float, color4_t>::iterator &iter);
	bool getUpperIterator(float &value, std::map<float, color4_t>::iterator &iter);
	
	std::map<float, color4_t> m_color_points;
		
};

}
}

#endif