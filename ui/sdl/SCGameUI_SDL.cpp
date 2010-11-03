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
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCObjectPrototypes.h"
#include "SCUnitCommand.h"
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
	SDL_Surface *tsf = TTF_RenderUTF8_Blended(font, text, *(SDL_Color*)&color);
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
	SDL_UnLockSurfaceIfNeeded(sf);
}

static SDL_Surface *g_juche_supplies_icon;
static SDL_Surface *g_sf_juche_aojiworker, *g_sf_juche_rodongcorrectionalfacility;

static grp_palette_t *g_palette_units;
static SDL_Surface *g_grp_icons;
static SDL_Surface *g_grp_minfield01, *g_grp_minfield01_shad;
static SDL_Surface *g_grp_wirefram;
static SDL_Surface *g_grp_t_ccenter, *g_grp_t_marine, *g_grp_t_firebat;
static SDL_Surface *g_grp_z_zergling, *g_grp_z_zergling_shad;

///

///////


GameUI_SDL::GameUI_SDL(Game *game, const PlayerSPtr_t &player)
	:GameUI(game, player)
{
	this->setFPS(100); // i want to set to 30 fps, but..
}

GameUI_SDL::~GameUI_SDL()
{
	//this->cleanupUI();
}

bool GameUI_SDL::initUI()
{
	int ret;
	
	if(unlikely(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0))
		throw new Exception("SDL_Init() failed");
	if(unlikely(TTF_Init() < 0))
		throw new Exception("TTF_Init() failed");
	
	SDL_Surface *screen;
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
	
	// game resources;
	this->m_sf_console = IMG_Load(GAME_ROOT_DIR "./res/ui/sdl/imgs/tconsole.png");
	
	// FIXME
	
	g_juche_supplies_icon = IMG_Load(GAME_ROOT_DIR "./res/ui/sdl/ingame/icons/juche_supplies_icon.jpg");
	g_sf_juche_rodongcorrectionalfacility = IMG_Load(GAME_ROOT_DIR "./res/ui/sdl/objects/buildings/53002.Juche_RodongCorrectionalFacility.jpg");
	g_sf_juche_aojiworker = IMG_Load(GAME_ROOT_DIR "./res/ui/sdl/objects/units/52001.Juche_AojiWorker.jpg");
	
	
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
	
	return true;
}

bool GameUI_SDL::cleanupUI()
{
	
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
				this->m_player->addToCommandQueue(UnitCommand(UnitCommandId::Attack));
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
			else if(ev.button.button == 1)
			{
				if(this->m_player->getFirstCommandInQueue().getCommandID() == UnitCommandId::Attack)
				{
					ObjectList dummy;
					ObjectSPtr_t first = this->m_game->findObjectByRect(dummy, x, y, x+10, y+10);
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
							(*it)->cmd_move(Coordinate(x, y), Object::MovementFlags::AutomaticallyAttack);
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
			if(ev.button.button == 1)
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
	const PlayerSPtr_t &me = this->m_player;
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
		if(this->m_player->getFirstCommandInQueue().getCommandID() == UnitCommandId::Attack)
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
		const ObjectSPtr_t &obj = *it;
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

static int calculate_unit_framenum(const ObjectSPtr_t &obj, int attack_start, int attack_end, int move_start, int move_end, 
		bool *do_hflip, bool *do_vflip)
{
	int col, row;
	*do_hflip = false;
	*do_vflip = false;
	
	row = convertAngleToDirection(obj->getAngle());
	time_t ticks = obj->getGame()->getCachedElapsedTime() * 1000;
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

void GameUI_SDL::drawObject(const ObjectSPtr_t &obj)
{
	const PlayerSPtr_t &player = this->m_player;
	int x, y, w, h;
	ObjectId_t objid = obj->getObjectId();
	const PlayerSPtr_t &owner = obj->getOwner();
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
		if(owner == player)
			ellipseRGBA(this->m_game_scr, x+w/2, y+h/2 +1, w/2 +1, h/2 +1, 0, 255, 0, 255);
		else if(owner->getRaceId() == RaceId::Neutral)
			ellipseRGBA(this->m_game_scr, x+w/2, y+h/2 +1, w/2 +1, h/2 +1, 255, 255, 0, 255);
		else
			ellipseRGBA(this->m_game_scr, x+w/2, y+h/2 +1, w/2 +1, h/2 +1, 255, 0, 0, 255);
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
void GameUI_SDL::drawObjects()
{
	ObjectList &objs = this->m_game->getObjectList();
	
	for(ObjectList::const_iterator it = objs.begin(); it != objs.end(); it++)
	{
		this->drawObject(*it);
	}
}
#endif






