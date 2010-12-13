/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *	
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *	
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/my_sdl.h
**/


#ifndef SC_MY_SDL_H_
#define SC_MY_SDL_H_

namespace SDL
{

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
//#include <SDL/SDL_gfxBlitFunc.h>


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
	
	void reset(SDL_Surface *sf)
	{
		this->SC::shared_ptr<SDL_Surface>::reset(sf, SDL_Surface_deleter());
	}
};

} /* END OF namespace SDL */

#endif

