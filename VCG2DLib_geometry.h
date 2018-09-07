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




#ifndef __VCG2DLIB_GOEMETRY_H
#define __VCG2DLIB_GOEMETRY_H


#include "VCG2DLib_prec.h"
#include <cmath> // abs, sqrt
#include <cassert> // assert
#include <algorithm> // max, min
#include <string>


#define ELINELINEINTERSECTION_NUM_ERROR_UP 1.00000001
#define ELINELINEINTERSECTION_NUM_ERROR_LOW 0.0


namespace VCG2D {
namespace Geometry {
	


int prepare_line_for_plotting(VCG2DVAR &x1, VCG2DVAR &y1, 
								VCG2DVAR &x2, VCG2DVAR &y2);


inline float ScalarProduct2Vectors(VCG2DVAR &v1x, 
									VCG2DVAR &v1y,
									 VCG2DVAR &v2x,
									  VCG2DVAR &v2y)
{
    return v1x*v2x + v1y*v2y;    
}

inline void NormalizeDirection(VCG2DVAR &dirx, VCG2DVAR &diry)
{
	
	VCG2DVAR invlenght = 1.0/sqrt(dirx*dirx + diry*diry);
	
	dirx *= invlenght;
	diry *= invlenght;
	
}

inline float VectorLength(VCG2DVAR &x1, VCG2DVAR &x2,
							VCG2DVAR &y1, VCG2DVAR &y2)
{

		float diffx = x2 - x1;
		float diffy = y2 - y1;
		
		return sqrt(diffx*diffx + diffy*diffy);
	
}

inline void getDirectionP1ToP2(VCG2DVAR &v1x, 
									VCG2DVAR &v1y,
									 VCG2DVAR &v2x,
									  VCG2DVAR &v2y,
										
										VCG2DVAR &dirx1x2,
										VCG2DVAR &diry1y2)
										
{
	dirx1x2 = v2x - v1x;
	diry1y2 = v2y - v1y;	
}


inline float getProjectedDistancePointLine(VCG2DVAR &v1x, 
									VCG2DVAR &v1y,
									 VCG2DVAR &v2x,
									  VCG2DVAR &v2y,
 
									 VCG2DVAR &px,
									  VCG2DVAR &py)
{

   VCG2DVAR dirx;
   VCG2DVAR diry;
  
   getDirectionP1ToP2(v1x, v1y, v2x, v2y, dirx, diry);
   
   VCG2DVAR invlength = 1.0/sqrt(dirx*dirx + diry*diry);

   
   dirx *= invlength;
   diry *= invlength;
   
   VCG2DVAR tmpx1 = px - v1x;
   VCG2DVAR tmpy1 = py - v1y;
   
   return (dirx*tmpx1 + diry*tmpy1) * invlength;
 
}

}}
#endif