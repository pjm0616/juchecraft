#ifndef SC_MY_SDL_H_
#define SC_MY_SDL_H_

namespace SDL
{

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
//#include <SDL/SDL_gfxBlitFunc.h>


//typedef SC::shared_ptr<SDL_Surface> SDL_SurfaceSPtr_t;
class SDL_SurfaceSPtr_t: public SC::shared_ptr<SDL_Surface>
{
public:
	struct SDL_Surface_deleter
	{
		void operator()(SDL_Surface *sf)
		{
			SDL_FreeSurface(sf);
		}
	};
	
	SDL_SurfaceSPtr_t()
	{
	}
	SDL_SurfaceSPtr_t(SDL_Surface *sf)
		:SC::shared_ptr<SDL_Surface>(sf, SDL_Surface_deleter())
	{
	}
};

} /* END OF namespace SDL */

#endif

