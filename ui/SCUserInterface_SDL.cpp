// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details



#ifdef DEBUG
#include <cstdio>
#include <cassert>
#endif

#include <tr1/memory>
#include <string>
#include <list>
#include <map>

#include <cstring>
#include <cstdlib>
#include <inttypes.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

namespace SDL
{
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
//#include <SDL/SDL_gfxBlitFunc.h>
}

#include "SCException.h"
#include "SCCoordinate.h"
#include "SCPlayer.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCGame.h"

#include "ui/SCUserInterface.h"
#include "ui/SCUserInterface_SDL.h"

#if 1
#include "libs/libmpqgrp/grp.cpp"
#endif


using namespace SDL;
using namespace SC;

std::map<ObjectId_t, std::string> UserInterface_SDL::obj_images;
bool UserInterface_SDL::is_initialized = false;


// filename: "ID.comment.txt"
static int check_resource_filename(const char *filename)
{
	const char *p = filename;
	while(isdigit(*p++)){}
	if(filename != p && *(p - 1) == '.')
		return atoi(filename);
	else
		return -1;
}

static void readfile(const char *filename, std::string &dest)
{
	int fd = open(filename, O_RDONLY);
	if(fd < 0)
		throw new Exception("open() failed");
	
	dest.clear();
	int nread;
	do
	{
		char buf[512];
		nread = read(fd, buf, sizeof(buf));
		if(nread > 0)
			dest.append(buf, nread);
	} while(nread > 0);
	close(fd);
}

static void replace_char(std::string &str, char needle, char replacement)
{
	size_t size = str.size();
	for(size_t i = 0; i < size; i++)
	{
		if(str[i] == needle)
			str[i] = replacement;
	}
}


/* static */
void UserInterface_SDL::load_resources(const char *dirpath)
{
	DIR *dp = opendir(dirpath);
	if(!dp)
		throw new Exception("opendir() failed");
	dirent *entry;
	while((entry = readdir(dp)) != NULL)
	{
		int id = check_resource_filename(entry->d_name);
		if(id >= 0)
		{
			std::string filepath(dirpath); filepath += entry->d_name;
			readfile(filepath.c_str(), UserInterface_SDL::obj_images[id]);
			replace_char(UserInterface_SDL::obj_images[id], '\n', '\0');
		}
	}
	closedir(dp);
	UserInterface_SDL::is_initialized = true;
}



/////////////////////

static inline int SDL_LockSurfaceIfNeeded(SDL_Surface *sf)
{
	if(SDL_MUSTLOCK(sf))
		return SDL_LockSurface(sf);
	else
		return 0;
}
static inline void SDL_UnlockSurfaceIfNeeded(SDL_Surface *sf)
{
	if(SDL_MUSTLOCK(sf))
		SDL_UnlockSurface(sf);
}

inline Uint32 *SDL_GetPixelPtr32(SDL_Surface *sf, int x, int y)
{
	return (Uint32 *)sf->pixels + y*sf->pitch/4 + x;
}

void DrawPixel(SDL_Surface *screen, int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
	Uint32 color = SDL_MapRGB(screen->format, R, G, B);
	
	//if(SDL_LockSurfaceIfNeeded(screen) < 0)
	//	return;
	
	#if 0
	switch (screen->format->BytesPerPixel)
	{
	case 1: { /* Assuming 8-bpp */
		Uint8 *bufp;

		bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
		*bufp = color;
		break;
	}

	case 2: { /* Probably 15-bpp or 16-bpp */
		Uint16 *bufp;

		bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
		*bufp = color;
		break;
	}

	case 3: { /* Slow 24-bpp mode, usually not used */
		Uint8 *bufp;

		bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
		*(bufp+screen->format->Rshift/8) = R;
		*(bufp+screen->format->Gshift/8) = G;
		*(bufp+screen->format->Bshift/8) = B;
		break;
	}

	case 4: { /* Probably 32-bpp */
	#endif
		Uint32 *bufp;

		bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
		*bufp = color;
	#if 0
		break;
	}}
	#endif
	
	//SDL_UnLockSurfaceIfNeeded(screen);
	//SDL_UpdateRect(screen, x, y, 1, 1);
}


static void SDL_FillSurfaceP(SDL_Surface *sf, int x, int y, int w, int h, unsigned int color)
{
	SDL_Rect r = {x, y, w, h};
	SDL_FillRect(sf, &r, color);
}

static void SDL_print(TTF_Font *font, SDL_Surface *sf, int x, int y, int w, int h, unsigned int color, const char *text)
{
	SDL_Surface *tsf = TTF_RenderUTF8_Blended(font, text, *(SDL_Color*)&color);
	SDL_Rect r = {x, y, w, h};
	SDL_BlitSurface(tsf, NULL, sf, &r);
	SDL_FreeSurface(tsf);
}


///////////
//

static void grp_setpix(void *surface, int x, int y, uint32_t color)
{
	Uint8 r = color&0xFF, g = (color&0xFF00)>>8, b = (color&0xFF0000)>>16;
	
	//printf("%d\t%d\t\t%d\t%d\t%d\n", x, y, r, g, b);
	//if(r==0xff && g==0x00 && b==0xff){
	//	r=0x00;b=0xff;g=0x00;
	//}else if(r==0xde && g==0x00 && b==0xde){
	//	r=0x00;b=0xff;g=0x00;
	//}
	
	return DrawPixel((SDL_Surface *)surface, x, y, r, g, b);
}
static grp_pixel_funcs g_grp_pixelfuncs={grp_setpix, NULL};


static void grp_setpix_wirefram_green(void *surface, int x, int y, uint32_t color)
{
	return DrawPixel((SDL_Surface *)surface, x, y, 0, 255, 0);
}
static grp_pixel_funcs g_grp_pixelfuncs_wirefram_green={grp_setpix_wirefram_green, NULL};
////



static grp_palette_t *g_palette_units;
static grp_data_t *g_grp_icons;
static grp_data_t *g_grp_minfield0;
static grp_data_t *g_grp_wirefram;
static grp_data_t *g_grp_t_ccenter, *g_grp_t_marine;


///


SDL_Surface *render_grp_frame(grp_data_t *grpdata, int framenum)
{
	grp_header_t *grpinfo = get_grp_info(grpdata);
	//grp_frameheader_t *frame = grp_get_frame_info(grpdata, framenum);
	SDL_Surface *sf = SDL_CreateRGBSurface(SDL_SWSURFACE, grpinfo->max_width, grpinfo->max_height, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
	
	SDL_LockSurfaceIfNeeded(sf);
	draw_grp(sf, 0, 0, &g_grp_pixelfuncs, grpdata, g_palette_units, framenum, 0, 0);
	SDL_UnlockSurfaceIfNeeded(sf);
	
	return sf;
}

SDL_Surface *render_grp_frame_flipped(grp_data_t *grpdata, int framenum, bool do_hflip = false, bool do_vflip = false)
{
	grp_header_t *grpinfo = get_grp_info(grpdata);
	grp_frameheader_t *frame = grp_get_frame_info(grpdata, framenum);
	SDL_Surface *sf = render_grp_frame(grpdata, framenum);
	int w = grpinfo->max_width;
	int h = grpinfo->max_height;
	int right = grpinfo->max_width-1 - frame->left;
	int bottom = grpinfo->max_height-1 - frame->top;
	
	if(do_hflip)
	{
		for(int y = 0; y < h; y++)
		{
			Uint32 *line = SDL_GetPixelPtr32(sf, 0, y);
			for(int x = 0; x < w/2; x++)
			{
				Uint32 tmp = line[x];
				line[x] = line[w-1 - x];
				line[w-1 - x] = tmp;
			}
		}
	}
	if(do_vflip)
	{
		int ymod = sf->pitch/4;
		for(int x = 0; x < w; x++)
		{
			Uint32 *col = (Uint32 *)sf->pixels + x;
			for(int y = 0; y < h/2; y++)
			{
				Uint32 tmp = col[y*ymod];
				col[y*ymod] = col[(h-1 - y)*ymod];
				col[(h-1 - y)*ymod] = tmp;
			}
		}
	}
	
	return sf;
}

void render_grp_frame_to_surface(grp_data_t *grpdata, int framenum, SDL_Surface *dest_sf, int x, int y, 
	int opt_align_w = -1, int opt_align_h = -1, bool do_hflip = false, bool do_yflip = false)
{
	grp_frameheader_t *frame = grp_get_frame_info(grpdata, framenum);
	
	SDL_Surface *rmodel = render_grp_frame_flipped(grpdata, framenum, do_hflip, do_yflip);
	SDL_Rect rmodel_rect = {frame->left, frame->top, frame->width, frame->height};
	SDL_Rect dest_rect = {x, y, frame->width, frame->height};
	//if(opt_align_w > 0)
	//	dest_rect.x -= (frame->width - opt_align_w);
	if(opt_align_h > 0)
		dest_rect.y -= (frame->height - opt_align_h);
	
	SDL_LockSurfaceIfNeeded(dest_sf);
	SDL_BlitSurface(rmodel, &rmodel_rect, dest_sf, &dest_rect);
	SDL_UnlockSurfaceIfNeeded(dest_sf);
	SDL_FreeSurface(rmodel);
}

///////


UserInterface_SDL::UserInterface_SDL(Game *game)
	:UserInterface(game)
{
	int ret;
	
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0)
		throw new Exception("SDL_Init() failed");
	if(TTF_Init() < 0)
		throw new Exception("TTF_Init() failed");
	
	SDL_Surface *screen;
	this->screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
	SDL_WM_SetCaption("주체크래프트", NULL);
	SDL_ShowCursor(SDL_ENABLE);
	
	this->font = TTF_OpenFont("./res/ui/sdl/fonts/NanumGothic.ttf", this->getFontSize());
	TTF_SetFontStyle(this->font, TTF_STYLE_NORMAL);
	
	this->game_scr = SDL_CreateRGBSurface(SDL_SWSURFACE, this->game->getMapWidth(), this->game->getMapHeight(), 32, 0, 0, 0, 0);
	this->gamescr_left_pos = 0;
	this->gamescr_top_pos = 0;
	
	this->minimap_wnd = SDL_CreateRGBSurface(SDL_SWSURFACE, 128, 128, 32, 0, 0, 0, 0);
	this->unitstat_wnd = SDL_CreateRGBSurface(SDL_SWSURFACE, 128, 128, 32, 0, 0, 0, 0);
	this->buttons_wnd = SDL_CreateRGBSurface(SDL_SWSURFACE, 128, 128, 32, 0, 0, 0, 0);
	
	// game resources;
	this->sf_console = IMG_Load("./res/ui/sdl/fonts/tconsole.png");
	
	// FIXME
	g_palette_units = load_palette("../mini_sc_data/libmpqgrp/sc_palettes/units.pal");
	g_grp_icons = load_grp("../mini_sc_data/StarDat/game/icons.grp");
	g_grp_minfield0 = load_grp("../mini_sc_data/StarDat/unit/neutral/min01.grp");
	g_grp_wirefram = load_grp("../mini_sc_data/StarDat/unit/wirefram/wirefram.grp");
	g_grp_t_ccenter = load_grp("../mini_sc_data/StarDat/unit/terran/control.grp");
	g_grp_t_marine = load_grp("../mini_sc_data/StarDat/unit/terran/marine.grp");
}

UserInterface_SDL::~UserInterface_SDL()
{
	SDL_FreeSurface(this->buttons_wnd);
	SDL_FreeSurface(this->unitstat_wnd);
	SDL_FreeSurface(this->buttons_wnd);
	
	SDL_FreeSurface(this->game_scr);
	SDL_FreeSurface(this->screen);
	
	TTF_Quit();
	SDL_Quit();
}

void UserInterface_SDL::processFrame()
{
	SDL_Event ev;
	
	for(int i = 0; i < 1000 && SDL_PollEvent(&ev); i++)
	{
		switch(ev.type)
		{
		case SDL_KEYDOWN: {
			// ev.key.
			switch(ev.key.keysym.sym)
			{
			case SDLK_UP:
				this->gamescr_top_pos -= 10;
				break;
			case SDLK_DOWN:
				this->gamescr_top_pos += 10;
				break;
			case SDLK_LEFT:
				this->gamescr_left_pos -= 10;
				break;
			case SDLK_RIGHT:
				this->gamescr_left_pos += 10;
				break;
			default:
				break;
			}
			break; }
		case SDL_KEYUP:
			// ev.key.
			break;
		case SDL_MOUSEMOTION:
			// ev.motion.
			break;
		case SDL_MOUSEBUTTONDOWN:
			// ev.button.
			if(ev.button.button == 3 && ev.button.state == SDL_PRESSED)
			{
				//fprintf(stderr, "move: %d, %d\n", ev.button.x, ev.button.y);
				SC::ObjectList::iterator it = this->game->getObjectList().begin();
				int x = this->gamescr_left_pos + ev.button.x;
				int y = this->gamescr_top_pos + ev.button.y;
				
				if(0)
				{
					x -= 8;
					y -= 12;
					it->get()->move(Coordinate(x, y));
				}
				else
				it->get()->move_centerAligned(Coordinate(x, y));
				
				//fprintf(stderr, "name:%s\n", it->get()->getObjectName());
			}
			break;
		case SDL_MOUSEBUTTONUP:
			// ev.button.
			break;
		case SDL_QUIT:
			std::exit(0);
			break;
		default:
			break;
		}
	}
}

void UserInterface_SDL::draw()
{
	SDL_FillSurfaceP(this->screen, 0, 0, 640, 480, 0x000000);
	
	// draw game screen
	{
		SDL_FillSurfaceP(this->game_scr, 0, 0, this->game->getMapWidth(), this->game->getMapHeight(), 0x111111);
		this->drawMap();
		this->drawObjects();
		{
			SDL_Rect sr = {this->gamescr_left_pos, this->gamescr_top_pos, 640, 480};
			SDL_Rect dr = {0, 0, 640, 480};
			SDL_BlitSurface(this->game_scr, &sr, this->screen, &dr);
		}
	}
	// draw ui
	this->drawUI();
	
	SDL_Flip(this->screen);
}


void UserInterface_SDL::drawUI()
{
	char buf[512];
	Player *me = &Player::Players[2];
	RaceId_t my_raceid = me->getRaceId();
	
	
	// 640-572=68
	// y: 3px
	// x: 640-68
	// w: 68
	// h: 14
	//mineral 436, 3
	//gas 504, 3
	//food 572, 3
	
	int y = 3;
	int x = 640 - 68;
	
	// draw food statistics
	for(int i = RaceId::Size-1; i >= 0 ; i--)
	{
		int max = me->getFoodMax(i);
		int crnt = me->getFoodCrnt(i);
		if(max > 0 || crnt > 0)
		{
			//fprintf(stderr, "x:%d, y:%d, race:%d, food:%d/%d\n", x, y, i, crnt, max);
			
			// draw icon
			{
				int frn = -1;
				if(i == RaceId::Zerg) frn = 4;
				else if(i == RaceId::Terran) frn = 5;
				else if(i == RaceId::Protoss) frn = 6;
				if(frn >= 0)
				{
					render_grp_frame_to_surface(g_grp_icons, frn, this->screen, x, y, -1, -1, false, false);
				}
				else
				{
					SDL_FillSurfaceP(this->screen, x, y, 14, 14, me->getPlayerColor());
				}
			}
			
			snprintf(buf, sizeof(buf), "%d/%d", me->getFoodCrnt(i), me->getFoodMax(i));
			SDL_print(this->font, this->screen, x + 15, y - 2, 68-15, 16, 0x00ff00, buf);
			x -= 68;
		}
	}
	// draw gas
	{
		//fprintf(stderr, "gas: x:%d, y:%d, race:%d\n", x, y);
		int frn = 2;
		if(my_raceid == RaceId::Zerg) frn = 1;
		else if(my_raceid == RaceId::Terran) frn = 2;
		else if(my_raceid == RaceId::Protoss) frn = 3;
		render_grp_frame_to_surface(g_grp_icons, frn, this->screen, x, y, -1, -1, false, false);
		
		snprintf(buf, sizeof(buf), "%d", me->getVespeneGas());
		SDL_print(this->font, this->screen, x + 15, y - 2, 68-15, 16, 0x00ff00, buf);
		x -= 68;
	}
	// draw minerals
	{
		//fprintf(stderr, "min: x:%d, y:%d, race:%d\n", x, y);
		int frn = 0;
		render_grp_frame_to_surface(g_grp_icons, frn, this->screen, x, y, -1, -1, false, false);
		
		snprintf(buf, sizeof(buf), "%d", me->getMinerals());
		SDL_print(this->font, this->screen, x + 15, y - 2, 68-15, 16, 0x00ff00, buf);
		x -= 68;
	}
	
	
	snprintf(buf, sizeof(buf), "FPS: %f  |  Frame#: %u", 
		game->getCurrentFPS(), game->getFrameNumber());
	SDL_print(this->font, this->screen, 0, 0, 640, 16, 0x00ff00, buf);
	
	
	// draw console area
	SDL_BlitSurface(this->sf_console, NULL, this->screen, NULL);
	this->drawUI_MinimapWnd();
	this->drawUI_UnitStatWnd();
	this->drawUI_ButtonsWnd();
}



void UserInterface_SDL::drawUI_MinimapWnd()
{
	// t-l: 6, 348
	// t-r: 133 348
	// b-l: 6 475
	// b-r: 133 475
	// size: 128x128
	
	static const int minimap_w = 128, minimap_h = 128;
	const int map_w = this->game->getMapWidth(), map_h = this->game->getMapHeight();
	// minimap_w : map_w = minimap_unit_w : unit_w
	float xratio = minimap_w / map_w, yratio = minimap_h / map_h;
	
	SDL_FillSurfaceP(this->minimap_wnd, 0, 0, minimap_w, minimap_h, 0x111111);
	
	ObjectList &objs = this->game->getObjectList();
	for(ObjectList::const_iterator it = objs.begin(); it != objs.end(); it++)
	{
		Object *obj = it->get();
		float unit_x = obj->getX(), unit_y = obj->getY();
		int unit_w = obj->getWidth(), unit_h = obj->getHeight();
		
		float mm_ux = unit_x * xratio;
		float mm_uy = unit_y * yratio;
		float mm_uw = unit_w * xratio;
		float mm_uh = unit_h * yratio;
	}
}

void UserInterface_SDL::drawUI_UnitStatWnd()
{
	SC::ObjectList::iterator it = this->game->getObjectList().begin();
	const char *name = it->get()->getObjectName();
	
	// left-top: 155, 390
	// right-bottom: 390, 470
	
	// unit name coord: 255,390
	//SDL_print(this->font, this->screen, 260, 390, (470-390)-255, this->getFontSize(), 0xffffff, name);
	SDL_print(this->font, this->screen, 260, 390, (470-390)-255, this->getFontSize(), 0xffffff, "주체 대포동발사병");
}

void UserInterface_SDL::drawUI_ButtonsWnd()
{
	
}

void UserInterface_SDL::drawMap()
{
	
}

const std::string *UserInterface_SDL::getObjectImg(ObjectId_t id) const
{
	const std::map<ObjectId_t, std::string> &obj_imgs = UserInterface_SDL::obj_images;
	std::map<ObjectId_t, std::string>::const_iterator it = obj_imgs.find(id);
	if(it == obj_images.end())
		return NULL;
	else
		return &it->second;
}

static int convertAngleToDirection(float angle)
{
	float angle2 = 360 - (angle - 90); // 0시 방향이 0도가 되도록 회전 후 좌-우를 뒤집음
	if(angle2 > 360) angle2 -= 360; // 0시 방향이 0도가 되도록 회전 후 기존0~90도 사이의 각들에 생긴 오류를 고침
	
	int direction = angle2 / (180.0/17); // 0~90 도 사이에 9개의 이미지가 있음. 10도당 이미지 한개
	
	//fprintf(stderr, "a: %f, a2: %f, dir: %d\n", angle, angle2, direction);
	return direction;
}

void UserInterface_SDL::drawObject(Object &obj)
{
	int x, y, w, h;
	ObjectId_t objid = obj.getObjectId();
	Player *owner = obj.getOwner();
	obj.getPosition(&x, &y);
	obj.getSize(&w, &h);
	
	#if 0
	#else
	//printf("Drawing object: %d,%d,%d,%d\t%x\n",x,y,w,h,owner->getPlayerColor());
	
	#if 1
	if(owner->getPlayerId() == 1)
	{
		SDL_FillSurfaceP(this->game_scr, x, y, w, h, owner->getPlayerColor());
		//SDL_FillSurfaceP(this->game_scr, x, y, w, h, owner->getPlayerColor());
		SDL_print(this->font, this->game_scr, x, y, w, h, 0xffffff, obj.getObjectName());
	}
	#endif
	
	#if 1
	if(objid == SC::ObjectId::Terran_Marine)
	{
		//ellipseColor(this->game_scr, x+w/2, y+h/2, w/2, h/2, (owner->getPlayerColor()<<8)|0xff);
		//ellipseRGBA(this->game_scr, x+w/2, y+h/2, w/2, h/2, 0, 255, 0, 255);
		
		ellipseRGBA(this->game_scr, x+w/2, y+h/2 +1, w/2 +1, h/2 +1, 0, 255, 0, 255);
	}
	#endif
	
		#if 1
		bool do_draw = false;
		grp_data_t *grpdata;
		short framenum;
		bool do_hflip = false;
		
		if(objid == SC::ObjectId::Terran_CommandCenter)
		{
			grpdata = g_grp_t_ccenter;
			framenum = 0;
			do_draw = true;
		}
		else if(objid == SC::ObjectId::Terran_Marine)
		{
			grpdata = g_grp_t_marine;
			int col = 0, row = 0;
			
			row = convertAngleToDirection(obj.getAngle());
			if(obj.isMoving())
				col = (lrand48()%(10-6))+6; // 6 ~ 10
			
			if(row == 34) // angle == 90deg
				row = 0;
			else if(row >= 0 && row <= 16){} // 오른쪽
			else if(row >= 17 && row <= 33) // 왼쪽
			{
				row = row - 17;
				row = 16 - row;
				do_hflip = true;
			}
			else
			{
				fprintf(stderr, "Cannot happen: row: %d; angle: %f\n", row, obj.getAngle());
			}
			
			framenum = col*17 + row;
			do_draw = true;
		}
		else if(objid == SC::ObjectId::Resource_MineralField)
		{
			grpdata = g_grp_minfield0;
			framenum = 0;
			do_draw = true;
		}
		
		if(do_draw)
		{
			{ // 이동시 마우스 포인터 중앙이 아래 점에 오도록 이동
				grp_header_t *grphdr = get_grp_info(grpdata);
				grp_frameheader_t *frame = grp_get_frame_info(grpdata, framenum);
				int center_x = (grphdr->max_width/2 - 1) - frame->left;
				int center_y = (grphdr->max_height/2 - 1) - frame->top;
			}
			
			render_grp_frame_to_surface(grpdata, framenum, this->game_scr, x, y, 0, h, do_hflip, false);
			#if 0
			grp_frameheader_t *frame = grp_get_frame_info(grpdata, framenum);
			
			SDL_Surface *rmodel = render_grp_frame_flipped(grpdata, framenum, do_hflip, false);
			SDL_Rect rmodel_rect = {frame->left, frame->top, frame->width, frame->height};
			SDL_Rect destrect = {x, y - (frame->height - h), frame->width, frame->height};
			
			SDL_LockSurfaceIfNeeded(this->game_scr);
			SDL_BlitSurface(rmodel, &rmodel_rect, this->game_scr, &destrect);
			//draw_grp(this->game_scr, x - frame->left, y - frame->top - (frame->height - h), &g_grp_pixelfuncs, grpdata, g_palette_units, framenum, 0, 0);
			SDL_UnlockSurfaceIfNeeded(this->game_scr);
			#endif
		}
		#endif
	#endif
}














