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



#ifndef _VCG2DLIB_TEXTURE_H_
#define _VCG2DLIB_TEXTURE_H_

struct texture_t {
	/// is about 30-20% slower compared to char when considering 
	/// rotation of buffer (memory jumps) and the application of various 
	/// effects. This would allow additional features (manipulating the texture with external libs etc.
	/// when no rotation is taken into account, the plotting would be about 5x faster usig
	/// usigned chars!
	float *rgba_array;
	
	int widht;
	int height;
};


#endif