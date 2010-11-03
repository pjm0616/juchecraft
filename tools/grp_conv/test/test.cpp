
#include <vector>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <assert.h>

namespace zlib
{
#include <zlib.h>
}

#include "../../../config.h"
#include "../../../smart_ptrs.h"
#include "../../../my_sdl.h"
#include "../../../libs/jcimg/jcimg.h"

using namespace SDL;

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface *scr = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
	
	
	JucheImage img;
	img.load("./test.dat", SDL_SWSURFACE);
	
	SDL_Surface *sf = img.getImage(0, NULL).get();
	
	SDL_BlitSurface(sf, NULL, scr, NULL);
	SDL_Flip(scr);
	
	SDL_Event ev;
	while(1)
	{
		if(SDL_WaitEvent(&ev))
		{
			if(ev.type == SDL_QUIT)
				break;
		}
	}
	SDL_Quit();
	
	return 0;
}














