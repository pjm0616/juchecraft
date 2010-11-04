
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

int main(int argc, char *argv[])
{
	const char *file = "./test.dat";
	if(argc >= 2)
		file = argv[1];
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface *scr = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
	
	JucheImage img;
	img.load(file, SDL_SWSURFACE);
	printf("# of images: %d\n", img.numOfImages());
	
	SDL_Surface *sf = img.getImage(0, NULL).get();
	SDL_BlitSurface(sf, NULL, scr, NULL);
	SDL_Flip(scr);
	
	int row = 0, col = 0;
	SDL_Event ev;
	while(1)
	{
		if(SDL_WaitEvent(&ev))
		{
			if(ev.type == SDL_QUIT)
				break;
			if(ev.type == SDL_KEYDOWN)
			{
				if(ev.key.keysym.sym == SDLK_UP)
					col++;
				else if(ev.key.keysym.sym == SDLK_DOWN)
					col--;
				else if(ev.key.keysym.sym == SDLK_LEFT)
					row--;
				else if(ev.key.keysym.sym == SDLK_RIGHT)
					row++;
				else
					continue;
				
				SDL_Rect s={0,0,640,480};
				SDL_FillRect(scr, &s, 0x00000000);
				
				int n = row*34 + col;
				if(n < 0 || n >= img.numOfImages())
				{
					printf("framenum out of range: %d / %d\n", n, img.numOfImages());
					continue;
				}
				
				SDL_Surface *sf = img.getImage(n, NULL).get();
				SDL_BlitSurface(sf, NULL, scr, NULL);
				SDL_Flip(scr);
				
				printf("row: %d, col: %d, n: %d\n", row, col, n);
				usleep(1000000/30);
			}
		}
	}
	SDL_Quit();
	
	return 0;
}














