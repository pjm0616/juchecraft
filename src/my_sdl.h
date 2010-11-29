// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SC_MY_SDL_H_
#define SC_MY_SDL_H_

namespace SDL
{

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
//#include <SDL/SDL_gfxBlitFunc.h>


//typedef SC::shared_ptr<SDL_Surface> SDL_SurfacePtr;
class SDL_SurfacePtr: public SC::shared_ptr<SDL_Surface>
{
public:
	struct SDL_Surface_deleter
	{
		void operator()(SDL_Surface *sf)
		{
			SDL_FreeSurface(sf);
		}
	};
	
	SDL_SurfacePtr()
	{
	}
	SDL_SurfacePtr(SDL_Surface *sf)
		:SC::shared_ptr<SDL_Surface>(sf, SDL_Surface_deleter())
	{
	}
};

} /* END OF namespace SDL */

#endif

