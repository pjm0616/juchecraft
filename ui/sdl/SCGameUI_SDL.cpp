// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"



#include "smart_ptrs.h"
#include <string>
#include <list>
#include <map>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <inttypes.h>
#include <cassert>

namespace SDL
{
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
//#include <SDL/SDL_gfxBlitFunc.h>
}

#include "defs.h"
#include "compat.h"
#include "luacpp/luacpp.h"
#include "SCException.h"
#include "SCCoordinate.h"
#include "SCPlayer.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCObjectIdList.h"
#include "SCObjectPrototypes.h"
#include "SCGame.h"

#include "ui/SCUserInterface.h"
#include "ui/SCUserInterface_SDL.h"

#if 1
#include "libmpqgrp/grp.h"
#include "libmpq/SFmpqapi/SFmpqapi.h"
#endif


using namespace SDL;
using namespace SC;

static inline Uint32 bswap32(Uint32 v)
{
	return (
			((v & 0xff000000) >> 24) | 
			((v & 0x00ff0000) >> 8) | 
			((v & 0x0000ff00) << 8) | 
			((v & 0x000000ff) << 24) 
			);
}

static inline Uint32 convert_color_from_bgra_to_rgba(Uint32 color_in_bgra)
{
	return (bswap32(color_in_bgra) >> 8) | (color_in_bgra & 0xff000000);
}

std::map<ObjectId_t, std::string> UserInterface_SDL::obj_images;
bool UserInterface_SDL::is_initialized = false;



/* static */
void UserInterface_SDL::load_resources(const char *dirpath)
{
	(void)dirpath;
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
static void SDL_BlitRectP(SDL_Surface *sf, int x, int y, int w, int h, unsigned int color)
{
	SDL_Surface *tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
	SDL_FillRect(tmp, NULL, color);
	SDL_Rect r = {x, y, w, h};
	SDL_BlitSurface(tmp, NULL, sf, &r);
	SDL_FreeSurface(tmp);
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

static void grp_setpix(void *surface, int x, int y, Uint32 color)
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


static void grp_setpix_wirefram_green(void *surface, int x, int y, Uint32 color)
{
	return DrawPixel((SDL_Surface *)surface, x, y, 0, 255, 0);
}
static grp_pixel_funcs g_grp_pixelfuncs_wirefram_green={grp_setpix_wirefram_green, NULL};
////


static SDL_Surface *g_juche_supplies_icon;
static SDL_Surface *g_sf_juche_aojiworker, *g_sf_juche_rodongcorrectionalfacility;

static grp_palette_t *g_palette_units;
static grp_data_t *g_grp_icons;
static grp_data_t *g_grp_minfield01, *g_grp_minfield01_shad;
static grp_data_t *g_grp_wirefram;
static grp_data_t *g_grp_t_ccenter, *g_grp_t_marine, *g_grp_t_firebat;
static grp_data_t *g_grp_z_zergling, *g_grp_z_zergling_shad;

///


SDL_Surface *render_grp_frame(grp_data_t *grpdata, int framenum, unsigned int grpflags = 0)
{
	grp_header_t *grpinfo = get_grp_info(grpdata);
	//grp_frameheader_t *frame = grp_get_frame_info(grpdata, framenum);
	SDL_Surface *sf = SDL_CreateRGBSurface(SDL_SWSURFACE, grpinfo->max_width, grpinfo->max_height, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
	
	SDL_LockSurfaceIfNeeded(sf);
	draw_grp(sf, 0, 0, &g_grp_pixelfuncs, grpdata, g_palette_units, framenum, grpflags, 0);
	SDL_UnlockSurfaceIfNeeded(sf);
	
	return sf;
}

SDL_Surface *render_grp_frame_flipped(grp_data_t *grpdata, int framenum, bool do_hflip = false, bool do_vflip = false, unsigned int grpflags = 0)
{
	grp_header_t *grpinfo = get_grp_info(grpdata);
	grp_frameheader_t *frame = grp_get_frame_info(grpdata, framenum);
	SDL_Surface *sf = render_grp_frame(grpdata, framenum, grpflags);
	int w = grpinfo->max_width;
	int h = grpinfo->max_height;
	
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

/* 유닛 색상을 비율에 따라 바꿔줌 */
void replace_unit_colors(SDL_Surface *sf, Uint32 newcolor)
{
	//static const Uint32 orig_unit_colors[] = {0xde00de, 0x5b005b, 0xbd00bd, 0x9c009c, 0x7c007c, 0x190019, 0xff00ff, 0x3a003a};
	Uint8 nc_r = (newcolor & 0x0000ff) >> 0;
	Uint8 nc_g = (newcolor & 0x00ff00) >> 8;
	Uint8 nc_b = (newcolor & 0xff0000) >> 16;
	
	for(int y = 0; y < sf->h; y++)
	{
		Uint32 *line = SDL_GetPixelPtr32(sf, 0, y);
		for(int x = 0; x < sf->w; x++)
		{
			Uint8 r, g, b, a;
			SDL_GetRGBA(line[x], sf->format, &r, &g, &b, &a);
			if(r != 0 && g == 0 && r == b)
			{
				// 0xff:orig_r = player_[rgb]:rendered_[rgb]
				Uint8 r2 = r * nc_r / 0xff;
				Uint8 g2 = r * nc_g / 0xff;
				Uint8 b2 = r * nc_b / 0xff;
				line[x] = SDL_MapRGBA(sf->format, r2, g2, b2, a);
			}
		}
	}
}

#define SHADOW_MAGIC_COLOR 0x00ef00ef
void render_shadow_image(SDL_Surface *sf)
{
	for(int y = 0; y < sf->h; y++)
	{
		Uint32 *line = SDL_GetPixelPtr32(sf, 0, y);
		for(int x = 0; x < sf->w; x++)
		{
			Uint8 r, g, b, a;
			SDL_GetRGBA(line[x], sf->format, &r, &g, &b, &a);
			if((b<<16 | g<<8 | r) == SHADOW_MAGIC_COLOR)
				line[x] = SDL_MapRGBA(sf->format, 0x00, 0x00, 0x00, 0xe0);
		}
	}
}

// this is VERY slow.
void render_grp_frame_to_surface(grp_data_t *grpdata, int framenum, SDL_Surface *dest_sf, int x, int y, 
	int opt_align_w = -1, int opt_align_h = -1, bool do_hflip = false, bool do_yflip = false, 
	Uint32 new_unit_color = 0x00000000, unsigned int grpflags = 0)
{
	grp_frameheader_t *frame = grp_get_frame_info(grpdata, framenum);
	
	SDL_Surface *rmodel = render_grp_frame_flipped(grpdata, framenum, do_hflip, do_yflip, grpflags);
	
	int new_x = x, new_y = y;
	int frame_left = frame->left, frame_top = frame->top;
	{
		/* 뒤집었을 경우 x, y 값을 보정해줌 */
		grp_header_t *grpinfo = get_grp_info(grpdata);
		if(do_hflip)
		{
			int new_left = (grpinfo->max_width-1) - (frame->left+frame->width);
			new_x -= (frame->left - new_left);
			frame_left = new_left;
		}
		if(do_yflip)
		{
			int new_top = (grpinfo->max_height-1) - (frame->top+frame->height);
			new_y -= (frame->top - new_top);
			frame_top = new_top;
		}
	}
	
	SDL_Rect rmodel_rect = {frame_left, frame_top, frame->width, frame->height};
	
	
	
	SDL_Rect dest_rect = {new_x, new_y, frame->width, frame->height};
	//if(opt_align_w > 0)
	//	dest_rect.x -= (frame->width - opt_align_w);
	if(opt_align_h > 0)
		dest_rect.y -= (frame->height - opt_align_h);
	
	if(new_unit_color != 0x00000000)
		replace_unit_colors(rmodel, new_unit_color);
	else if(grpflags & SHADOW_COLOR)
		render_shadow_image(rmodel);
	
	SDL_LockSurfaceIfNeeded(dest_sf);
	SDL_BlitSurface(rmodel, &rmodel_rect, dest_sf, &dest_rect);
	SDL_UnlockSurfaceIfNeeded(dest_sf);
	SDL_FreeSurface(rmodel);
}


///////


UserInterface_SDL::UserInterface_SDL(Game *game)
	:UserInterface(game)
{
	// FIXME: hack
#ifdef __WIN32__
	this->setFPS(30); // 30 fps
#else
	this->setFPS(100); // 30 fps
#endif
	
	this->clearCommandToBeOrdered();
}

UserInterface_SDL::~UserInterface_SDL()
{
	//this->cleanupUI();
}

bool UserInterface_SDL::initUI()
{
	int ret;
	
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0)
		throw new Exception("SDL_Init() failed");
	if(TTF_Init() < 0)
		throw new Exception("TTF_Init() failed");
	
	SDL_Surface *screen;
	this->m_screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
	SDL_WM_SetCaption("JucheCraft", NULL);
	SDL_ShowCursor(SDL_ENABLE);
	
	this->m_font = TTF_OpenFont(GAME_ROOT_DIR "./res/ui/sdl/fonts/NanumGothic.ttf", this->getFontSize());
	TTF_SetFontStyle(this->m_font, TTF_STYLE_NORMAL);
	
	// color format: 0xAABBGGRR; 0xffBBGGRR
	this->m_game_scr = SDL_CreateRGBSurface(SDL_HWSURFACE, this->m_game->getMapWidth(), this->m_game->getMapHeight(), 
		32, 0xff, 0xff00, 0xff0000, 0xff000000);
	
	this->m_gamescr_left_pos = 0;
	this->m_gamescr_top_pos = 0;
	
	this->m_minimap_wnd = SDL_CreateRGBSurface(SDL_SWSURFACE, 128, 128, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
	this->m_unitstat_wnd = SDL_CreateRGBSurface(SDL_SWSURFACE, 128, 128, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
	this->m_buttons_wnd = SDL_CreateRGBSurface(SDL_SWSURFACE, 128, 128, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
	
	// game resources;
	this->m_sf_console = IMG_Load(GAME_ROOT_DIR "./res/ui/sdl/imgs/tconsole.png");
	
	// FIXME
	
	g_juche_supplies_icon = IMG_Load(GAME_ROOT_DIR "./res/ui/sdl/ingame/icons/juche_supplies_icon.jpg");
	g_sf_juche_rodongcorrectionalfacility = IMG_Load(GAME_ROOT_DIR "./res/ui/sdl/objects/buildings/53002.Juche_RodongCorrectionalFacility.jpg");
	g_sf_juche_aojiworker = IMG_Load(GAME_ROOT_DIR "./res/ui/sdl/objects/units/52001.Juche_AojiWorker.jpg");
	
	HANDLE mpq_handles[3];
	SFileOpenArchive(GAME_DATA_DIR "./StarDat.mpq", 1000, 0, &mpq_handles[0]);
	SFileOpenArchive(GAME_DATA_DIR "./BrooDat.mpq", 2000, 0, &mpq_handles[1]);
	SFileOpenArchive(GAME_DATA_DIR "./patch_rt.mpq", 3000, 0, &mpq_handles[2]);
	
	#if 0
	#if 0
	grp_set_file_method(GRP_USE_FILE);
	g_palette_units = load_palette(GAME_DATA_DIR "./libmpqgrp/sc_palettes/units.pal");
	grp_set_file_method(GRP_USE_MPQ);
	#else
	g_palette_units = load_palette(GAME_DATA_DIR "./libmpqgrp/sc_palettes/units.pal");
	#endif
	g_grp_icons = load_grp("game/icons.grp");
	g_grp_minfield01 = load_grp("unit/neutral/min01.grp");
	g_grp_minfield01_shad = load_grp("unit/neutral/min01sha.grp");
	g_grp_wirefram = load_grp("unit/wirefram/wirefram.grp");
	g_grp_t_ccenter = load_grp("unit/terran/control.grp");
	g_grp_t_marine = load_grp("unit/terran/marine.grp");
	g_grp_t_firebat = load_grp("unit/terran/firebat.grp");
	g_grp_z_zergling = load_grp("unit/zerg/zergling.grp");
	g_grp_z_zergling_shad = load_grp("unit/zerg/zzeshad.grp");
	#else
	grp_set_file_method(GRP_USE_MPQ);
	g_palette_units = load_palette("tileset\\Platform.wpe");
	g_grp_icons = load_grp("game\\icons.grp");
	g_grp_minfield01 = load_grp("unit\\neutral\\min01.grp");
	g_grp_minfield01_shad = load_grp("unit\\neutral\\min01sha.grp");
	g_grp_wirefram = load_grp("unit\\wirefram\\wirefram.grp");
	g_grp_t_ccenter = load_grp("unit\\terran\\control.grp");
	g_grp_t_marine = load_grp("unit\\terran\\marine.grp");
	g_grp_t_firebat = load_grp("unit\\terran\\firebat.grp");
	g_grp_z_zergling = load_grp("unit\\zerg\\zergling.grp");
	g_grp_z_zergling_shad = load_grp("unit\\zerg\\zzeshad.grp");
	#endif
	
	return true;
}

bool UserInterface_SDL::cleanupUI()
{
	this->m_selected_objs.clear();
	
	// do SFileCloseArchive
	// do free(g_grp_*)
	
	// free game resources
	SDL_FreeSurface(this->m_sf_console);
	
	// free game surfaces
	SDL_FreeSurface(this->m_buttons_wnd);
	SDL_FreeSurface(this->m_unitstat_wnd);
	SDL_FreeSurface(this->m_minimap_wnd);
	
	SDL_FreeSurface(this->m_game_scr);
	SDL_FreeSurface(this->m_screen);
	
	TTF_CloseFont(this->m_font);
	
	TTF_Quit();
	SDL_Quit();
	
	return true;
}


bool UserInterface_SDL::isSelectedUnit(const ObjectSPtr_t &obj)
{
	for(ObjectList::const_iterator it = this->m_selected_objs.begin(); 
		it != this->m_selected_objs.end(); ++it)
	{
		if(*it == obj)
			return true;
	}
	return false;
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
				this->m_gamescr_top_pos -= 10;
				break;
			case SDLK_DOWN:
				this->m_gamescr_top_pos += 10;
				break;
			case SDLK_LEFT:
				this->m_gamescr_left_pos -= 10;
				break;
			case SDLK_RIGHT:
				this->m_gamescr_left_pos += 10;
				break;
			case 'a':
				this->setCommandToBeOrdered(2);
				break;
			default:
				break;
			}
			break; }
		case SDL_KEYUP:
			// ev.key.
			break;
		case SDL_MOUSEMOTION: {
			// ev.motion.
			int x = this->m_gamescr_left_pos + ev.button.x;
			int y = this->m_gamescr_top_pos + ev.button.y;
			this->m_mouse_pos_in_gamescr.set(x, y);
			break;
		}
		case SDL_MOUSEBUTTONDOWN: {
			// ev.button.
			int x = this->m_gamescr_left_pos + ev.button.x;
			int y = this->m_gamescr_top_pos + ev.button.y;
			this->m_mouse_pos_in_gamescr.set(x, y);
			if(ev.button.button == 3)
			{
				//fprintf(stderr, "move: %d, %d\n", ev.button.x, ev.button.y);
				
				for(ObjectList::const_iterator it = this->m_selected_objs.begin(); 
					it != this->m_selected_objs.end(); ++it)
				{
					#if 0
					if(0)
					{
						x -= 8;
						y -= 12;
						(*it)->move_notAligned(Coordinate(x, y));
					}
					else
					#endif
					(*it)->cmd_move(Coordinate(x, y));
				}
			}
			else if(ev.button.button == 1)
			{
				if(this->getCommandToBeOrdered() == 2)
				{
					ObjectList selected_objs;
					ObjectSPtr_t first = this->m_game->findObjectByRect(selected_objs, x, y, x+10, y+10);
				
					if(first)
					{
						for(ObjectList::const_iterator it = this->m_selected_objs.begin(); 
							it != this->m_selected_objs.end(); ++it)
						{
							(*it)->cmd_attack(first);
						}
					}
					else
					{
						for(ObjectList::const_iterator it = this->m_selected_objs.begin(); 
							it != this->m_selected_objs.end(); ++it)
						{
							(*it)->cmd_move(Coordinate(x, y), Object::MovementFlags::AutomaticallyAttack);
						}
					}
					this->clearCommandToBeOrdered();
				}
				else
				{
					this->m_selection_start_coordinate.set(x, y);
					this->m_selection_in_progress = true;
				}
			}
			break;
		}
		case SDL_MOUSEBUTTONUP: {
			// ev.button.
			int x = this->m_gamescr_left_pos + ev.button.x;
			int y = this->m_gamescr_top_pos + ev.button.y;
			this->m_mouse_pos_in_gamescr.set(x, y);
			if(ev.button.button == 1)
			{
				if(this->m_selection_in_progress)
				{
					this->m_selection_in_progress = false;
					Coordinate selection_end(x, y);
					this->m_game->findObjectByRect(this->m_selected_objs, this->m_selection_start_coordinate, selection_end);
					{
						int stats[ObjectType::SIZE] = {0, };
						for(ObjectList::const_iterator it = this->m_selected_objs.begin(); 
							it != this->m_selected_objs.end(); ++it)
						{
							const ObjectSPtr_t &obj = *it;
							stats[obj->getObjectType()]++;
						}
					
						/* 유닛이 하나 이상 있다면 유닛 빼고 다른 물체는 선택하지 않음 */
						if(stats[ObjectType::Unit] > 0)
						{
							for(ObjectList::iterator it = this->m_selected_objs.begin(); 
								it != this->m_selected_objs.end(); )
							{
								const ObjectSPtr_t &obj = *it;
								if(obj->getObjectType() != ObjectType::Unit)
									this->m_selected_objs.erase(it++);
								else
									++it;
							}
						}
					}
					if(this->m_selection_start_coordinate == selection_end)
					{
						ObjectList::iterator end = this->m_selected_objs.end();
						ObjectList::iterator it = this->m_selected_objs.begin();
						if(it != end)
							++it;
						while(it != end)
							this->m_selected_objs.erase(it++);
					}
				}
			}
			break;
		}
		case SDL_QUIT:
			//std::exit(0);
			this->m_game->endGame();
			break;
		default:
			break;
		}
	}
}

#define MAP_COLOR 0xff808080
void UserInterface_SDL::draw()
{
	SDL_FillSurfaceP(this->m_screen, 0, 0, 640, 480, 0x00000000); // 0xffffff
	
	// draw game screen
	{
		SDL_FillSurfaceP(this->m_game_scr, 0, 0, this->m_game->getMapWidth(), this->m_game->getMapHeight(), MAP_COLOR);
		this->drawMap();
		this->drawObjects(); // THIS IS VERY SLOW. avg. 0.02secs
		// etc..
		{
			if(this->m_selection_in_progress)
			{
				Coordinate tl(this->m_selection_start_coordinate), br(this->m_mouse_pos_in_gamescr);
				Coordinate::normalizeTopLeftCoordinate(tl, br);
				
				int w = br.getX() - tl.getX(), h = br.getY() - tl.getY();
				SDL_BlitRectP(this->m_game_scr, tl.getX(), tl.getY(), w, h, 0x4000ff00);
				rectangleRGBA(this->m_game_scr, tl.getX(), tl.getY(), br.getX(), br.getY(), 0, 255, 0, 255);
			}
		}
		// blit game screen
		{
			SDL_Rect sr = {this->m_gamescr_left_pos, this->m_gamescr_top_pos, 640, 480};
			SDL_Rect dr = {0, 0, 640, 480};
			SDL_BlitSurface(this->m_game_scr, &sr, this->m_screen, &dr);
		}
	}
	// draw ui
	this->drawUI();
	
	SDL_Flip(this->m_screen);
}


void UserInterface_SDL::drawUI()
{
	char buf[512];
	Game *game = this->m_game;
	Player *me = &Player::Players[1];
	RaceId_t my_raceid = me->getRaceId();
	
	// 640-572=68
	// y: 3px
	// x: 640-68
	// w: 68
	// h: 14
	//mineral 436, 3
	//gas 504, 3
	//supplies 572, 3
	int y = 3;
	int x = 640 - 68;
	
	// draw supply statistics
	for(int i = RaceId::Size-1; i >= 0 ; i--)
	{
		int max = me->getCurrentSupplies(i);
		int crnt = me->getSuppliesInUse(i);
		if(max > 0 || crnt > 0)
		{
			//fprintf(stderr, "x:%d, y:%d, race:%d, supplies:%d/%d\n", x, y, i, crnt, max);
			
			// draw icon
			{
				int frn = -1;
				if(i == RaceId::Zerg) frn = 4;
				else if(i == RaceId::Terran) frn = 5;
				else if(i == RaceId::Protoss) frn = 6;
				if(frn >= 0)
				{
					render_grp_frame_to_surface(g_grp_icons, frn, this->m_screen, x, y, -1, -1, false, false);
				}
				else
				{
					if(i == RaceId::Juche)
					{
						SDL_Rect r = {x, y, 14, 14};
						SDL_BlitSurface(g_juche_supplies_icon, NULL, this->m_screen, &r);
					}
					else
					{
						SDL_FillSurfaceP(this->m_screen, x, y, 14, 14, me->getPlayerColor());
					}
				}
			}
			
			// select color
			Uint32 color;
			if(crnt <= max)
				color = 0xff00ff00; // green
			else
				color = 0xff0000ff; // red
			
			// draw to screen
			snprintf(buf, sizeof(buf), "%d/%d", me->getSuppliesInUse(i), me->getCurrentSupplies(i));
			SDL_print(this->m_font, this->m_screen, x + 15, y - 2, 68-15, 16, color, buf);
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
		render_grp_frame_to_surface(g_grp_icons, frn, this->m_screen, x, y, -1, -1, false, false);
		
		snprintf(buf, sizeof(buf), "%d", me->getVespeneGas());
		SDL_print(this->m_font, this->m_screen, x + 15, y - 2, 68-15, 16, 0xff00ff00, buf);
		x -= 68;
	}
	// draw minerals
	{
		//fprintf(stderr, "min: x:%d, y:%d, race:%d\n", x, y);
		int frn = 0;
		render_grp_frame_to_surface(g_grp_icons, frn, this->m_screen, x, y, -1, -1, false, false);
		
		snprintf(buf, sizeof(buf), "%d", me->getMinerals());
		SDL_print(this->m_font, this->m_screen, x + 15, y - 2, 68-15, 16, 0xff00ff00, buf);
		x -= 68;
	}
	
	snprintf(buf, sizeof(buf), "Rate: %.02f |  FPS: %.02f |  Frame#: %u", 
		game->getCurrentUpdateRate(), game->getCurrentFPS(), game->getFrameNumber());
	SDL_print(this->m_font, this->m_screen, 0, 0, 640, 16, 0xff00ff00, buf);
	
	// draw console area
	SDL_BlitSurface(this->m_sf_console, NULL, this->m_screen, NULL); // this takes avg 0.006 secs
	this->drawUI_MinimapWnd();
	this->drawUI_UnitStatWnd();
	this->drawUI_ButtonsWnd();
	
	{	
		if(this->getCommandToBeOrdered() == 2)
		{
			SDL_print(this->m_font, this->m_screen, 210+50, 330, 150, 16, 0xffffffff, "Select target");
		}
	}
}



void UserInterface_SDL::drawUI_MinimapWnd()
{
	// t-l: 6, 348
	// t-r: 133 348
	// b-l: 6 475
	// b-r: 133 475
	// size: 128x128
	
	static const int minimap_w = 128, minimap_h = 128;
	const int map_w = this->m_game->getMapWidth(), map_h = this->m_game->getMapHeight();
	// minimap_w : map_w = minimap_unit_w : unit_w
	float xratio = minimap_w / map_w, yratio = minimap_h / map_h;
	
	SDL_FillSurfaceP(this->m_minimap_wnd, 0, 0, minimap_w, minimap_h, MAP_COLOR);
	
	ObjectList &objs = this->m_game->getObjectList();
	for(ObjectList::const_iterator it = objs.begin(); it != objs.end(); it++)
	{
		const ObjectSPtr_t &obj = *it;
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
	SC::ObjectList::iterator it = this->m_selected_objs.begin();
	while(it != this->m_selected_objs.end())
	{
		const ObjectSPtr_t &obj = *it++;
		if(obj->isRemovedFromGame() == true) // warning: hack
			continue;
		
		// left-top: 155, 390
		// right-bottom: 390, 470
		
		// unit name coord: 255,390
		{
			const char *name = obj->getObjectName();
			SDL_print(this->m_font, this->m_screen, 235, 390, (470-390)-255, this->getFontSize(), 0xffffffff, name);
		}
		
		// Unit HP
		// 160,450
		{
			char buf[64];
			snprintf(buf, sizeof(buf), "%.0f / %d", obj->getHP(), obj->getMaxHP());
			SDL_print(this->m_font, this->m_screen, 160, 450, 100, this->getFontSize(), 0xff00ff00, buf);
		}
		
		break;
	}
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
	float angle2 = 360 - (angle - 90); /* 0시 방향이 0도가 되도록 회전 후 좌-우를 뒤집음 */
	if(angle2 > 360) angle2 -= 360; /* 0시 방향이 0도가 되도록 회전 후 기존0~90도 사이의 각들에 생긴 오류를 고침 */
	
	int direction = angle2 / (180.0/17); /* 0~90 도 사이에 9개의 이미지가 있음. 10도당 이미지 한개 */
	
	//fprintf(stderr, "a: %f, a2: %f, dir: %d\n", angle, angle2, direction);
	return direction;
}

static int calculate_unit_framenum(const ObjectSPtr_t &obj, int attack_start, int attack_end, int move_start, int move_end, 
		bool *do_hflip, bool *do_vflip)
{
	int col, row;
	*do_hflip = false;
	*do_vflip = false;
	
	row = convertAngleToDirection(obj->getAngle());
	time_t ticks = obj->getGame()->getLastTicks();
	if(obj->isMoving())
	{
		int t = (int)(ticks/50) % (move_end - move_start + 1);
		col = t + move_start;
	}
	else if(obj->isAttacking())
	{
		int t = (int)(ticks/80) % (attack_end - attack_start + 1);
		col = t + attack_start;
	}
	else
		col = move_start;
	
	if(row == 34) // angle == 90deg
		row = 0;
	else if(row >= 0 && row <= 16){} /* 오른쪽 */
	else if(row >= 17 && row <= 33) /* 왼쪽 */
	{
		row = row - 17;
		row = 16 - row;
		*do_hflip = true;
	}
	else
	{
		fprintf(stderr, "Cannot happen: row: %d; angle: %f\n", row, obj->getAngle());
		row = 0;
	}
	
	int framenum = col*17 + row;
	return framenum;
}

void UserInterface_SDL::drawObject(const ObjectSPtr_t &obj)
{
	Player *player = &Player::Players[1];
	int x, y, w, h;
	ObjectId_t objid = obj->getObjectId();
	Player *owner = obj->getOwner();
	obj->getPosition(&x, &y);
	obj->getSize(&w, &h);
	
	#if 0
	#else
	//printf("Drawing object: %d,%d,%d,%d\t%x\n",x,y,w,h, owner->getPlayerColor());
	
	#if 0
	if(owner->getPlayerId() == 1)
	{
		//SDL_FillSurfaceP(this->m_game_scr, x, y, w, h, convert_color_from_bgra_to_rgba(owner->getPlayerColor()));
		SDL_print(this->m_font, this->m_game_scr, x, y, w, h, 0xffffffff, obj->getObjectName());
	}
	#endif
	
	#if 1
	//if(objid == SC::ObjectId::Juche_DaepodongLauncher)
	if(this->m_selection_in_progress)
	{
		if(owner == player && obj->insideRect(this->m_selection_start_coordinate, this->m_mouse_pos_in_gamescr))
		{
			ellipseRGBA(this->m_game_scr, x+w/2, y+h/2 +1, w/2 +1, h/2 +1, 0, 255, 0, 255);
		}
	}
	else
	{
		if(this->isSelectedUnit(obj))
		{
			if(owner == player)
				ellipseRGBA(this->m_game_scr, x+w/2, y+h/2 +1, w/2 +1, h/2 +1, 0, 255, 0, 255);
			else if(owner == &Player::Players[Player::NeutralPlayer])
				ellipseRGBA(this->m_game_scr, x+w/2, y+h/2 +1, w/2 +1, h/2 +1, 255, 255, 0, 255);
			else
				ellipseRGBA(this->m_game_scr, x+w/2, y+h/2 +1, w/2 +1, h/2 +1, 255, 0, 0, 255);
		}
	}
	#endif
	
		#if 1
		bool do_draw = false;
		grp_data_t *grpdata = NULL, *grpdata2 = NULL;
		SDL_Surface *sf1 = NULL;
		short framenum;
		bool do_hflip = false, do_vflip = false;
		Uint32 unit_color = convert_color_from_bgra_to_rgba(owner->getPlayerColor());
		
		if(objid == SC::ObjectId::Terran_CommandCenter)
		{
			grpdata = g_grp_t_ccenter;
			framenum = 0;
			do_draw = true;
		}
		else if((objid == SC::ObjectId::Terran_Marine) || (objid == SC::ObjectId::Juche_DaepodongLauncher))
		{
			grpdata = g_grp_t_marine;
			// 0~1: attack preparation images
			// 13: dying image
			framenum = calculate_unit_framenum(obj, 2, 3, 4, 12, &do_hflip, &do_vflip);
			do_draw = true;
		}
		else if(objid == SC::ObjectId::Terran_Firebat)
		{
			grpdata = g_grp_t_firebat;
			framenum = calculate_unit_framenum(obj, 0, 1, 2, 9, &do_hflip, &do_vflip);
			do_draw = true;
		}
		else if(objid == SC::ObjectId::Zerg_Zergling)
		{
			grpdata = g_grp_z_zergling;
			grpdata2 = g_grp_z_zergling_shad;
			framenum = calculate_unit_framenum(obj, 0, 3, 4, 11, &do_hflip, &do_vflip);
			do_draw = true;
		}
		else if(objid == SC::ObjectId::Resource_MineralField)
		{
			grpdata = g_grp_minfield01;
			grpdata2 = g_grp_minfield01_shad;
			framenum = 0;
			do_draw = true;
		}
		else if(objid == SC::ObjectId::Juche_AojiWorker)
		{
			sf1 = g_sf_juche_aojiworker;
			do_draw = true;
		}
		else if(objid == SC::ObjectId::Juche_RodongCorrectionalFacility)
		{
			sf1 = g_sf_juche_rodongcorrectionalfacility;
			do_draw = true;
		}
		
		if(do_draw)
		{
			if(grpdata)
			{
				{ /* 이동시 마우스 포인터 중앙이 아래 점에 오도록 이동 */
					grp_header_t *grphdr = get_grp_info(grpdata);
					grp_frameheader_t *frame = grp_get_frame_info(grpdata, framenum);
					int center_x = (grphdr->max_width/2 - 1) - frame->left;
					int center_y = (grphdr->max_height/2 - 1) - frame->top;
				}
			
				if(grpdata2) // shadow
					render_grp_frame_to_surface(grpdata2, framenum, this->m_game_scr, x, y, 0, h, do_hflip, false, 0x00000000, SHADOW_COLOR|(SHADOW_MAGIC_COLOR<<8));
				if(grpdata)
					render_grp_frame_to_surface(grpdata, framenum, this->m_game_scr, x, y, 0, h, do_hflip, false, unit_color);
			}
			if(sf1)
			{
				SDL_Rect r = {x, y, w, h};
				SDL_BlitSurface(sf1, NULL, this->m_game_scr, &r);
			}
		}
		#endif
	#endif
}





#ifndef DRAW_OBJECTS_WITH_VIRTUAL_FXNS
void UserInterface_SDL::drawObjects()
{
	ObjectList &objs = this->m_game->getObjectList();
	
	for(ObjectList::const_iterator it = objs.begin(); it != objs.end(); it++)
	{
		this->drawObject(*it);
	}
}
#endif






