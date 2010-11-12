// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"



#include "smart_ptrs.h"
#include <string>
#include <list>
#include <map>
#include <vector>
#include <deque>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <inttypes.h>
#include <cassert>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "defs.h"
#include "compat.h"
#include "luacpp/luacpp.h"
#include "SCTypes.h"
#include "SCException.h"
#include "SCCoordinate.h"
#include "SCObjectIdList.h"
#include "actions/UnitAction.h"
#include "SCUnitCommand.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCObjectFactory.h"
#include "SCPlayer.h"
#include "SCGame.h"

#include "ui/SCGameUI.h"
#include "ui/sdl/SCGameUI_SDL.h"

#include "my_sdl.h"
#include "jcimg/jcimg.h"



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

std::map<ObjectId_t, std::string> GameUI_SDL::obj_images;
bool GameUI_SDL::is_initialized = false;



/* static */
void GameUI_SDL::load_resources(const char *dirpath)
{
	(void)dirpath;
	GameUI_SDL::is_initialized = true;
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

static inline Uint32 *SDL_GetPixelPtr32(SDL_Surface *sf, int x, int y)
{
	return (Uint32 *)sf->pixels + y*sf->pitch/4 + x;
}
static inline void SDL_SetPixel32(SDL_Surface *sf, int x, int y, Uint32 color)
{
	Uint32 *pixel = SDL_GetPixelPtr32(sf, x, y);
	*pixel = color;
}
static inline Uint32 SDL_MapRGBA32(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	return (a << 24) | (b << 16) | (g << 8) | (r);
}
static inline void SDL_GetRGBA32(Uint32 color, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a)
{
	*r = (color & 0x000000ff) >> 0;
	*g = (color & 0x0000ff00) >> 8;
	*b = (color & 0x00ff0000) >> 16;
	*a = (color & 0xff000000) >> 24;
}

static void SDL_FillSurfaceP(SDL_Surface *sf, int x, int y, int w, int h, unsigned int color)
{
	SDL_Rect r = {x, y, w, h};
	SDL_FillRect(sf, &r, color);
}
static void SDL_BlitRectP(SDL_Surface *sf, int x, int y, int w, int h, unsigned int color)
{
	SDL_Surface *tmp = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
	SDL_FillRect(tmp, NULL, color);
	SDL_Rect r = {x, y, w, h};
	SDL_BlitSurface(tmp, NULL, sf, &r);
	SDL_FreeSurface(tmp);
}

static void SDL_print(TTF_Font *font, SDL_Surface *sf, int x, int y, int w, int h, unsigned int color, const char *text)
{
	SDL_Color sdlcolor = {color&0xff, (color&0xff00) >> 8, (color&0xff0000) >> 16, 0};
	SDL_Surface *tsf = TTF_RenderUTF8_Blended(font, text, sdlcolor);
	SDL_Rect r = {x, y, w, h};
	SDL_BlitSurface(tsf, NULL, sf, &r);
	SDL_FreeSurface(tsf);
}



/* 유닛 색상을 비율에 따라 바꿔줌 */
// This is slow :(
void replace_unit_colors(SDL_Surface *sf, Uint32 newcolor)
{
	SDL_LockSurfaceIfNeeded(sf);
	//static const Uint32 orig_unit_colors[] = {0xde00de, 0x5b005b, 0xbd00bd, 0x9c009c, 0x7c007c, 0x190019, 0xff00ff, 0x3a003a};
	Uint8 nc_r = (newcolor & 0x0000ff) >> 0;
	Uint8 nc_g = (newcolor & 0x00ff00) >> 8;
	Uint8 nc_b = (newcolor & 0xff0000) >> 16;
	
	#pragma omp parallel for if(sf->h > 200) shared(sf)
	for(int y = 0; y < sf->h; y++)
	{
		Uint32 *line = SDL_GetPixelPtr32(sf, 0, y);
		for(int x = 0; x < sf->w; x++)
		{
			Uint8 r, g, b, a;
			SDL_GetRGBA32(line[x], &r, &g, &b, &a);
			if(r != 0 && g == 0 && r == b)
			{
				// 0xff:orig_r = player_[rgb]:rendered_[rgb]
				Uint8 r2 = r * nc_r / 0xff;
				Uint8 g2 = r * nc_g / 0xff;
				Uint8 b2 = r * nc_b / 0xff;
				line[x] = SDL_MapRGBA32(r2, g2, b2, a);
			}
		}
	}
	SDL_UnlockSurfaceIfNeeded(sf);
}


static JucheImage	g_img_game_tconsole, 
					g_img_game_icon_mineral, 
					g_img_game_icon_gas_terran, 
					g_img_game_icon_gas_protoss, 
					g_img_game_icon_gas_zerg, 
					g_img_game_icon_supplies_terran, 
					g_img_game_icon_supplies_protoss, 
					g_img_game_icon_supplies_zerg, 
					g_img_game_icon_supplies_juche;

static JucheImage	g_img_obj_terran_unit_marine, 
					g_img_obj_terran_unit_marine_attack, 
					g_img_obj_terran_building_command_center, 
					g_img_obj_zerg_unit_zergling, 
					g_img_obj_zerg_unit_zergling_shadow, 
					g_img_obj_zerg_unit_zergling_attack, 
					g_img_obj_zerg_unit_zergling_attack_shadow, 
					
					g_img_obj_juche_unit_aojiworker, 
					g_img_obj_juche_building_rodongcorrectionalfacility, 
					g_img_obj_neutral_mineral01, 
					g_img_obj_neutral_mineral01_shadow, 
					g_img_end;

//	const SDL::SDL_SurfacePtr &getImage(unsigned int n, jcimg_imginfo_t **imginfo = NULL);
static void render_jcimg_to_screen(JucheImage *img, SDL_Surface *scr, int num, int scr_x, int scr_y)
{
	jcimg_imginfo_t *imginfo;
	SDL_SurfacePtr sf = img->getImage(num, &imginfo);
	
	SDL_Rect srcrect = {imginfo->left, imginfo->top, imginfo->width, imginfo->height};
	SDL_Rect dstrect = {scr_x, scr_y, imginfo->width, imginfo->height};
	
	SDL_BlitSurface(sf.get(), &srcrect, scr, &dstrect);
}
static void render_jcimg_obj_to_screen(const ObjectPtr &obj, JucheImage *img, SDL_Surface *scr, int num, int scr_cx, int scr_cy)
{
	jcimg_imginfo_t *imginfo;
	SDL_SurfacePtr sf = img->getImage(num, &imginfo);
	
	// TODO
	#if 1
	//@{
	int img_centerx = (img->getMaxWidth()-1) / 2;
	int img_centery = (img->getMaxHeight()-1) / 2;
	//@}
	
	int left = scr_cx - img_centerx + imginfo->left;
	int top = scr_cy - img_centery + imginfo->top;
	#endif
	
	SDL_Rect srcrect = {imginfo->left, imginfo->top, imginfo->width, imginfo->height};
	SDL_Rect dstrect = {left, top, imginfo->width, imginfo->height};
	
	SDL_BlitSurface(sf.get(), &srcrect, scr, &dstrect);
}

///////


GameUI_SDL::GameUI_SDL(Game *game, Player *player)
	:GameUI(game, player)
{
	this->setFPS(100); // i want to set to 30 fps, but..
}

GameUI_SDL::~GameUI_SDL()
{
	//this->cleanupUI();
}

bool GameUI_SDL::loadResources()
{
	g_img_game_tconsole.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/game/tconsole.dat", SDL_HWSURFACE);
	g_img_game_icon_mineral.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/game/icons/mineral.dat", SDL_HWSURFACE);
	g_img_game_icon_gas_terran.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/game/icons/gas_terran.dat", SDL_HWSURFACE);
	g_img_game_icon_gas_protoss.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/game/icons/gas_protoss.dat", SDL_HWSURFACE);
	g_img_game_icon_gas_zerg.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/game/icons/gas_zerg.dat", SDL_HWSURFACE);
	g_img_game_icon_supplies_terran.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/game/icons/supplies_terran.dat", SDL_HWSURFACE);
	g_img_game_icon_supplies_protoss.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/game/icons/supplies_protoss.dat", SDL_HWSURFACE);
	g_img_game_icon_supplies_zerg.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/game/icons/supplies_zerg.dat", SDL_HWSURFACE);
	g_img_game_icon_supplies_juche.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/game/icons/supplies_juche.dat", SDL_HWSURFACE);
	

	g_img_obj_terran_unit_marine.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/objects/terran/units/marine_walk.dat", SDL_HWSURFACE);
	g_img_obj_terran_unit_marine_attack.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/objects/terran/units/marine_attack.dat", SDL_HWSURFACE);
	g_img_obj_terran_building_command_center.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/objects/terran/buildings/command_center.dat", SDL_HWSURFACE);
	g_img_obj_zerg_unit_zergling.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/objects/zerg/units/zergling_walk.dat", SDL_HWSURFACE);
	g_img_obj_zerg_unit_zergling_shadow.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/objects/zerg/units/zergling_walk_shadow.dat", SDL_HWSURFACE);
	g_img_obj_zerg_unit_zergling_attack.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/objects/zerg/units/zergling_attack.dat", SDL_HWSURFACE);
	g_img_obj_zerg_unit_zergling_attack_shadow.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/objects/zerg/units/zergling_attack_shadow.dat", SDL_HWSURFACE);

	g_img_obj_juche_unit_aojiworker.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/objects/juche/units/52001.Juche_AojiWorker.dat", SDL_HWSURFACE);
	g_img_obj_juche_building_rodongcorrectionalfacility.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/objects/juche/buildings/53002.Juche_RodongCorrectionalFacility.dat", SDL_HWSURFACE);
	g_img_obj_neutral_mineral01.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/objects/neutral/mineral01.dat", SDL_HWSURFACE);
	g_img_obj_neutral_mineral01_shadow.load(GAME_ROOT_DIR "./res/ui/sdl/imgs/objects/neutral/mineral01_shadow.dat", SDL_HWSURFACE);
	
	return 0;
}

bool GameUI_SDL::initUI()
{
	if(unlikely(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0))
		throw new Exception("SDL_Init() failed");
	if(unlikely(TTF_Init() < 0))
		throw new Exception("TTF_Init() failed");
	
	this->m_screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("JucheCraft", NULL);
	SDL_ShowCursor(SDL_ENABLE);
	
	this->m_font = TTF_OpenFont(GAME_ROOT_DIR "./res/ui/sdl/fonts/NanumGothic.ttf", this->getFontSize());
	TTF_SetFontStyle(this->m_font, TTF_STYLE_NORMAL);
	
	// color format: 0xAABBGGRR; 0xffBBGGRR
	this->m_game_scr = SDL_CreateRGBSurface(SDL_HWSURFACE, this->m_game->getMapWidth(), this->m_game->getMapHeight(), 
		32, 0xff, 0xff00, 0xff0000, 0xff000000);
	
	this->m_gamescr_left_pos = 0;
	this->m_gamescr_top_pos = 0;
	
	this->m_minimap_wnd = SDL_CreateRGBSurface(SDL_HWSURFACE, 128, 128, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
	this->m_unitstat_wnd = SDL_CreateRGBSurface(SDL_HWSURFACE, 128, 128, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
	this->m_buttons_wnd = SDL_CreateRGBSurface(SDL_HWSURFACE, 128, 128, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
	
	this->loadResources();
	
	return true;
}

bool GameUI_SDL::cleanupUI()
{
	
	// do SFileCloseArchive
	// do free(g_grp_*)
	
	// free game resources
	// TODO
	
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


void GameUI_SDL::processFrame()
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
				this->m_player->addToCommandQueue(UnitCommand::Command(UnitCommand::CommandId::Attack));
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
			if(ev.button.button == 3) // right button
			{
				//fprintf(stderr, "move: %d, %d\n", ev.button.x, ev.button.y);
				
				const ObjectList &selected_objs = this->m_player->getSelectedObjs();
				for(ObjectList::const_iterator it = selected_objs.begin(); 
					it != selected_objs.end(); ++it)
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
			else if(ev.button.button == 1) // left button
			{
				if(this->m_player->getFirstCommandInQueue().getCommandID() == UnitCommand::CommandId::Attack)
				{
					ObjectList dummy;
					ObjectPtr first = this->m_game->findObjectByRect(dummy, x, y, x+10, y+10);
					const ObjectList &selected_objs = this->m_player->getSelectedObjs();
				
					if(first)
					{
						for(ObjectList::const_iterator it = selected_objs.begin(); 
							it != selected_objs.end(); ++it)
						{
							(*it)->cmd_attack(first);
						}
					}
					else
					{
						for(ObjectList::const_iterator it = selected_objs.begin(); 
							it != selected_objs.end(); ++it)
						{
							(*it)->cmd_move(Coordinate(x, y), UnitAction::Move::MovementFlags::AutomaticallyAttack);
						}
					}
					this->m_player->clearCommandQueue();
				}
				else
				{
					this->m_player->startObjectSelection(Coordinate(x, y));
				}
			}
			break;
		}
		case SDL_MOUSEBUTTONUP: {
			// ev.button.
			int x = this->m_gamescr_left_pos + ev.button.x;
			int y = this->m_gamescr_top_pos + ev.button.y;
			this->m_mouse_pos_in_gamescr.set(x, y);
			if(ev.button.button == 1) // left button
			{
				if(this->m_player->isSelectionInProgress())
				{
					this->m_player->finishObjectSelection(Coordinate(x, y));
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
void GameUI_SDL::draw()
{
	SDL_FillSurfaceP(this->m_screen, 0, 0, 640, 480, 0x00000000); // 0xffffff
	
	// draw game screen
	{
		SDL_FillSurfaceP(this->m_game_scr, 0, 0, this->m_game->getMapWidth(), this->m_game->getMapHeight(), MAP_COLOR);
		this->drawMap();
		this->drawObjects(); // THIS IS VERY SLOW. avg. 0.02secs
		// etc..
		{
			if(this->m_player->isSelectionInProgress())
			{
				Coordinate tl(this->m_player->getSelectionStartCoordinate()), br(this->m_mouse_pos_in_gamescr);
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


void GameUI_SDL::drawUI()
{
	char buf[512];
	Game *game = this->m_game;
	Player *me = this->m_player;
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
				JucheImage *ico = NULL;
				if(i == RaceId::Zerg) ico = &g_img_game_icon_supplies_zerg;
				else if(i == RaceId::Terran) ico = &g_img_game_icon_supplies_terran;
				else if(i == RaceId::Protoss) ico = &g_img_game_icon_supplies_protoss;
				else if(i == RaceId::Juche) ico = &g_img_game_icon_supplies_juche;
				if(ico != NULL)
				{
					render_jcimg_to_screen(ico, this->m_screen, 0, x, y);
				}
				else
				{
					SDL_FillSurfaceP(this->m_screen, x, y, 14, 14, me->getPlayerColor());
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
		JucheImage *ico = &g_img_game_icon_gas_terran;
		if(my_raceid == RaceId::Zerg) ico = &g_img_game_icon_gas_zerg;
		else if(my_raceid == RaceId::Terran) ico = &g_img_game_icon_gas_terran;
		else if(my_raceid == RaceId::Protoss) ico = &g_img_game_icon_gas_protoss;
		render_jcimg_to_screen(ico, this->m_screen, 0, x, y);
		
		snprintf(buf, sizeof(buf), "%d", me->getVespeneGas());
		SDL_print(this->m_font, this->m_screen, x + 15, y - 2, 68-15, 16, 0xff00ff00, buf);
		x -= 68;
	}
	// draw minerals
	{
		//fprintf(stderr, "min: x:%d, y:%d, race:%d\n", x, y);
		render_jcimg_to_screen(&g_img_game_icon_mineral, this->m_screen, 0, x, y);
		
		snprintf(buf, sizeof(buf), "%d", me->getMinerals());
		SDL_print(this->m_font, this->m_screen, x + 15, y - 2, 68-15, 16, 0xff00ff00, buf);
		x -= 68;
	}
	
	snprintf(buf, sizeof(buf), "Rate: %.02f |  FPS: %.02f |  Frame#: %u", 
		game->getCurrentUpdateRate(), game->getCurrentFPS(), game->getFrameNumber());
	SDL_print(this->m_font, this->m_screen, 0, 0, 640, 16, 0xff00ff00, buf);
	
	// draw console area
	SDL_BlitSurface(g_img_game_tconsole.getImage(0).get(), NULL, this->m_screen, NULL); // this takes avg 0.006 secs
	this->drawUI_MinimapWnd();
	this->drawUI_UnitStatWnd();
	this->drawUI_ButtonsWnd();
	
	{	
		if(this->m_player->getFirstCommandInQueue().getCommandID() == UnitCommand::CommandId::Attack)
		{
			SDL_print(this->m_font, this->m_screen, 210+50, 330, 150, 16, 0xffffffff, "Select target");
		}
	}
}



void GameUI_SDL::drawUI_MinimapWnd()
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
		const ObjectPtr &obj = *it;
		float unit_x = obj->getX(), unit_y = obj->getY();
		int unit_w = obj->getWidth(), unit_h = obj->getHeight();
		
		float mm_ux = unit_x * xratio;
		float mm_uy = unit_y * yratio;
		float mm_uw = unit_w * xratio;
		float mm_uh = unit_h * yratio;
	}
}

void GameUI_SDL::drawUI_UnitStatWnd()
{
	const ObjectList &selected_objs = this->m_player->getSelectedObjs();
	SC::ObjectList::const_iterator it = selected_objs.begin();
	while(it != selected_objs.end())
	{
		const ObjectPtr &obj = *it++;
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

void GameUI_SDL::drawUI_ButtonsWnd()
{
	
}

void GameUI_SDL::drawMap()
{
	
}

const std::string *GameUI_SDL::getObjectImg(ObjectId_t id) const
{
	const std::map<ObjectId_t, std::string> &obj_imgs = GameUI_SDL::obj_images;
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

static int calculate_unit_framenum(const ObjectPtr &obj, int start, int end)
{
	int col, row;
	
	row = convertAngleToDirection(obj->getAngle());
	time_t ticks = obj->getGame()->getCachedElapsedTime() * 1000;
	if(obj->isMoving())
	{
		int t = (int)(ticks/50) % (end - start + 1);
		col = t + start;
	}
	else if(obj->isAttacking())
	{
		int t = (int)(ticks/80) % (end - start + 1);
		col = t + start;
	}
	else
		col = start;
	
	if(row == 34) // angle == 90deg
		row = 0;
	else if(row >= 0 && row <= 33){}
	else
	{
		fprintf(stderr, "Cannot happen: row: %d; angle: %f\n", row, obj->getAngle());
		row = 0;
	}
	
	int framenum = col*34 + row;
	return framenum;
}

void GameUI_SDL::drawObject(const ObjectPtr &obj)
{
	Player *player = this->m_player;
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
	//(this->m_player->isSelectionInProgress() && obj->insideRect(this->m_player->getSelectionStartCoordinate(), this->m_mouse_pos_in_gamescr))
	if(this->m_player->isSelectedObject(obj))
	{
		uint8_t r, g, b;
		if(owner == player)
			(r = 0), (g = 255), (b = 0);
		else if(owner->getRaceId() == RaceId::Neutral)
			(r = 255), (g = 255), (b = 0);
		else
			(r = 255), (g = 0), (b = 0);
		
		ellipseRGBA(this->m_game_scr, x, y +4, w/2 +1, h/2 +1, r, g, b, 255);
	}
	#endif
	
		#if 1
		bool do_draw = false;
		JucheImage *img1 = NULL, *img1_shad = NULL;
		short framenum = -1;
		Uint32 unit_color = convert_color_from_bgra_to_rgba(owner->getPlayerColor());
		
		if(objid == SC::ObjectId::Terran_CommandCenter)
		{
			img1 = &g_img_obj_terran_building_command_center;
			framenum = 0;
			do_draw = true;
		}
		else if((objid == SC::ObjectId::Terran_Marine) || (objid == SC::ObjectId::Juche_DaepodongLauncher))
		{
			if(obj->isAttacking())
			{
				img1 = &g_img_obj_terran_unit_marine_attack;
				framenum = calculate_unit_framenum(obj, 0, 1);
			}
			else
			{
				img1 = &g_img_obj_terran_unit_marine;
				framenum = calculate_unit_framenum(obj, 0, 8);
			}
			do_draw = true;
		}
		#if 0
		else if(objid == SC::ObjectId::Terran_Firebat)
		{
			if(obj->isAttacking())
			{
				img1 = &g_img_obj_terran_unit_firebat_attack;
				framenum = calculate_unit_framenum(obj, 0, 1);
			}
			else
			{
				img1 = &g_img_obj_terran_unit_firebat;
				framenum = calculate_unit_framenum(obj, 0, 7);
			}
			do_draw = true;
		}
		#endif
		else if(objid == SC::ObjectId::Zerg_Zergling)
		{
			if(obj->isAttacking())
			{
				img1 = &g_img_obj_zerg_unit_zergling_attack;
				img1_shad = &g_img_obj_zerg_unit_zergling_attack_shadow;
				framenum = calculate_unit_framenum(obj, 0, 3);
			}
			else
			{
				img1 = &g_img_obj_zerg_unit_zergling;
				img1_shad = &g_img_obj_zerg_unit_zergling_shadow;
				framenum = calculate_unit_framenum(obj, 0, 7);
			}
			do_draw = true;
		}
		else if(objid == SC::ObjectId::Resource_MineralField)
		{
			img1 = &g_img_obj_neutral_mineral01;
			img1_shad = &g_img_obj_neutral_mineral01_shadow;
			framenum = 0;
			do_draw = true;
		}
		else if(objid == SC::ObjectId::Juche_AojiWorker)
		{
			img1 = &g_img_obj_juche_unit_aojiworker;
			framenum = 0;
			do_draw = true;
		}
		else if(objid == SC::ObjectId::Juche_RodongCorrectionalFacility)
		{
			img1 = &g_img_obj_juche_building_rodongcorrectionalfacility;
			framenum = 0;
			do_draw = true;
		}
		
		if(do_draw)
		{
			if(img1)
			{
				#if 0
				{ /* 이동시 마우스 포인터 중앙이 아래 점에 오도록 이동 */
					jcimg_imginfo_t *imginfo;
					img1->getImage(framenum, &imginfo);
					int center_x = (img1->getMaxWidth()/2 - 1) - imginfo->left;
					int center_y = (img1->getMaxHeight()/2 - 1) - imginfo->top;
				}
				#endif
				
				if(img1_shad) // shadow
					render_jcimg_obj_to_screen(obj, img1_shad, this->m_game_scr, framenum, x, y);
				if(img1)
					render_jcimg_obj_to_screen(obj, img1, this->m_game_scr, framenum, x, y);
			}
		}
		#endif
	#endif
}





#ifndef DRAW_OBJECTS_WITH_VIRTUAL_FXNS
void GameUI_SDL::drawObjects()
{
	ObjectList &objs = this->m_game->getObjectList();
	
	for(ObjectList::const_iterator it = objs.begin(); it != objs.end(); it++)
	{
		this->drawObject(*it);
	}
}
#endif






