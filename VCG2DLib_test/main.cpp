/*
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
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <ctime>

#include "../VCG2DLib_prec.h"
#include "../VCG2DLib.h"
#include "../VCG2DLib_colors.h"
#include "../VCG2DLib_gradients.h"
#include "../VCG2DLib_fill.h"

#include "SDL_config.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include <SDL2/SDL_mixer.h>


SDL_Window *g_mainWindow;
SDL_GLContext g_openGLContext;

float g_debug_offsetx = 300;
float g_debug_offsety = -200;


/// this is a function to test a none very well tested function:
/// prepare_line_for_plotting_firstpoint, at the moment it is
///
void waterfronttest()
{
	VCG2DVAR x1 = 400.000;
	VCG2DVAR y1 = 555.000;

	VCG2DVAR x2 = -100.000;
	VCG2DVAR y2 = 400.000;

	bool fixedone = false;
	bool fixedtwo = false;


	VCG2D::print_current_screen_information();

	int result = VCG2D::prepare_line_for_plotting_firstpoint(x1, y1,
								x2, y2, fixedone, fixedtwo);
	if(result != 0)
		VCG2D::prepare_line_for_plotting_firstpoint(x2, y2,
								x1, y1, fixedtwo, fixedone);

	std::cout << x1 << std::endl;
	std::cout << y1 << std::endl;
	std::cout << x2 << std::endl;
	std::cout << y2 << std::endl;

	std::cout << fixedone << std::endl;
	std::cout << fixedtwo << std::endl;

	std::cout << "result:" << result << std::endl;

	int testingit = 0;
}

int main(int argc, char** argv)
{

    SDL_Event event;
	Uint32 format;

	int window_widht = 100;
	int window_height = 100;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window = SDL_CreateWindow("VCG2DLib demo",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_widht,
									window_height, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    /// PixelBuffer * framebuffer = createPixelBuffer( VCG2D::g_screen_width, VCG2D::g_screen_height );
	SDL_Texture *texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, window_widht,
		window_height);
    Uint32 * pixels = new Uint32[window_widht * window_height];

	int w, h;
	SDL_QueryTexture(texture, &format, nullptr, &w, &h);

	SDL_PixelFormat pixelFormat;
	pixelFormat.format = format;

	char R = 100;
	char G = 0;
	char B = 0;


	VCG2D::init_screen_buffers(1, 0, 0, window_widht, window_height, 4*sizeof(char), SCREEN_INTERNAL_PIXELSIZE_COLORANDALPHA);
    memset(pixels, 0, window_height * window_widht * sizeof(Uint32));

	VCG2D::set_currentscreen_index(0);

	VCG2D::Gradients::gradient2d_t gradient;

	gradient.m_x1 = 0;
	gradient.m_y1 = 0;
    gradient.m_x2 = 100;
	gradient.m_y2 = 100;

	VCG2D::color4_t tcol;
	gradient.m_color_points[0.0] = tcol;
	gradient.m_color_points[1.0] = tcol;

	gradient.m_color_points[0.0].r = 0;
	gradient.m_color_points[0.0].g = 0;
	gradient.m_color_points[0.0].b = 0;
	gradient.m_color_points[0.0].a = 1;

	gradient.m_color_points[1.0].r = 1.0;
	gradient.m_color_points[1.0].g = 1.0;
	gradient.m_color_points[1.0].b = 1.0;
	gradient.m_color_points[1.0].a = 1.0;


	unsigned char tmp1 = 0;
	VERSION_BUFFER_TYPE tmpv = 0;
	float tmpf = 0;
	int skiptmp = 0;
	VERSION_BUFFER_TYPE vindex = 0;

    while (true)
    {
    /// reset the version index
        vindex = 0;

		VCG2D::memset_currentscreen_buffer( tmp1 );
		VCG2D::memset_currentscreen_version_buffer( tmpv );
		VCG2D::memset_currentscreen_fbuffer( tmpf );
		VCG2D::memset_currentscreen_skip_buffer( skiptmp );


		unsigned int version = 1;
		unsigned int versionbefore = version;
		//poly.drawVersionToCurrentScreen(version);


		VCG2D::color4_t colt;
		colt.r = 0;
		colt.g = 1;
		colt.b = 0;
		colt.a = 1;
		version = 1;
		colt.r = 1;

		VCG2D::color4_t colt1;
		colt.r = 0;
		colt.g = 0;
		colt.b = 1;
		colt.a = 1;





		++vindex;

		VCG2DVAR x1,y1,x2,y2,x3,y3,x4,y4,x5,y5;

		x1 = 0;
		y1 = 0;

		x2 = 50;
		y2 = 50;

		x3 = 0;
		y3 = 50;

/// prepares for filling
		VCG2D::Lines::draw_line_setversion(x1, y1, x2, y2, vindex);
		VCG2D::Lines::draw_line_setversion(x2, y2, x3, y3, vindex);
		VCG2D::Lines::draw_line_setversion(x3, y3, x1, y1, vindex);

/// filling has to be done anti clock wise !! Fill the prepared outlined polygone (This has to be a polygon at all times !)
		VCG2D::Fill::fill_line_onecolor(x2, y2, x3, y3, colt, vindex);
		VCG2D::Fill::fill_line_onecolor(x3, y3, x1, y1, colt, vindex);
		VCG2D::Fill::fill_line_onecolor(x1, y1, x2, y2, colt, vindex);

		/// now draw the triangle corners
		VCG2D::Lines::draw_singleline_onecolor(x1, y1, x2, y2, colt1);
		VCG2D::Lines::draw_singleline_onecolor(x2, y2, x3, y3, colt1);
		VCG2D::Lines::draw_singleline_onecolor(x3, y3, x1, y1, colt1);

/// increment the version by 4 (every polygon takes 4 indices on the backbuffer)
        vindex += 4;

        x1 = 0;
        y1 = 0;

        x2 = 10;
        y2 = 2;

        x3 = 90;
        y3 = 50;

        x4 = 90;
        y4 = 90;

        VCG2D::Splines::draw_cubic_bazier_line_setversion(x1,y1, x2,y2, x3, y3, x4,y4, vindex);

        x5 = 50;
        y5 = 50;

/// counter clockwise !
        VCG2D::Lines::draw_line_setversion(x5, y5, x1, y1, vindex);
        VCG2D::Lines::draw_line_setversion(x4, y4, x5, y5, vindex);

/// fill the polygon !
/// filling has to be done anti clock wise !! Fill the prepared outlined polygone (This has to be a polygon at all times !)
		VCG2D::Fill::fill_line_onecolor_gradient(x4, y4, x5, y5, gradient, vindex);
		VCG2D::Fill::fill_line_onecolor_gradient(x5, y5, x1, y1, gradient, vindex);


/// now draw the triangle corners
		VCG2D::Lines::draw_singleline_onecolor(x1, y1, x5, y5, colt1);
		VCG2D::Lines::draw_singleline_onecolor(x5, y5, x4, y4, colt1);
		VCG2D::Splines::draw_cubic_bazier_line_onecolor(x1,y1, x2,y2, x3, y3, x4,y4, colt1);

/// copy current screen into final psoido texture buffer
        VCG2D::copy2RGBA8888Buffer(pixels);
/// copy final psoido texture buffer into SDL texture
		SDL_UpdateTexture(texture, NULL, pixels, window_widht * sizeof(Uint32));


/// publish the texture
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);


		/*switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
		}*/
    }

    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();

	VCG2D::free_screen_buffers();

    return 0;

}
