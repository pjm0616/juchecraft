/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/ui/ncurses/GameUI_ncurses.cpp
**/

#include "config.h"

#ifndef NO_NCURSES_UI

#include "smart_ptrs.h"
#include <string>
#include <list>
#include <map>
#include <vector>
#include <deque>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

namespace ncurses
{
#include <ncurses.h>
}

#include "defs.h"
#include "compat.h"
#include "luacpp/luacpp.h"
#include "game/Types.h"
#include "game/Exception.h"
#include "game/Coordinate.h"
#include "game/Target.h"
#include "game/ObjectIdList.h"
#include "game/actions/Actions.h"
#include "game/orders/Orders.h"
#include "game/Object.h"
#include "game/ObjectList.h"
#include "game/ObjectFactory.h"
#include "game/Player.h"
#include "game/Game.h"

#include "ui/GameUI.h"
#include "ui/ncurses/GameUI_ncurses.h"

using namespace ncurses; // DAMN....  COLOR_PAIR macro requires this
using namespace SC;

std::map<ObjectId_t, std::string> GameUI_ncurses::obj_images;
bool GameUI_ncurses::is_initialized = false;


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


void GameUI_ncurses::load_resources(const char *dirpath)
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
			readfile(filepath.c_str(), GameUI_ncurses::obj_images[id]);
			replace_char(GameUI_ncurses::obj_images[id], '\n', '\0');
		}
	}
	closedir(dp);
	GameUI_ncurses::is_initialized = true;
}


/***** 120x36 matrix
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
*****/


//#define SC_NCURSES_UNIT_COLOR_BASE 20
#define SC_NCURSES_UNIT_COLOR_BASE 0
#define SC_NCURSES_UNIT_COLOR_PAIR_BASE 20


GameUI_ncurses::GameUI_ncurses(Game *game, Player *player)
	:GameUI(game, player)
{
	
	this->setFPS(8);
}

GameUI_ncurses::~GameUI_ncurses()
{
	//this->cleanupUI();
}

bool GameUI_ncurses::initUI()
{
	int ret;
	
	// initialize
	ncurses::initscr();
	ret = ncurses::start_color();
	if(ret == ERR)
	{
		ncurses::endwin();
		throw new Exception("Terminal does not support color.");
	}
	ncurses::noecho();
	ncurses::cbreak();
	ncurses::keypad(ncurses::stdscr, true);
	
	if(ncurses::has_mouse())
	{
		this->m_has_mouse = true;
		//ncurses::mousemask(ALL_MOUSE_EVENTS|REPORT_MOUSE_POSITION|BUTTON_SHIFT|BUTTON_CTRL|BUTTON_ALT, NULL);
		ncurses::mousemask(ALL_MOUSE_EVENTS, NULL);
	}
	else
	{
		//ncurses::endwin(); throw new Exception("Terminal does not support mouse.");
		this->m_has_mouse = false;
	}
	
	// check screen size
	getmaxyx(ncurses::stdscr, this->m_win_y, this->m_win_x);
	if(this->m_win_x < 120 || this->m_win_y < 36)
	{
		ncurses::endwin();
		throw new Exception("Window size is too small. (min: 120x36)");
	}
	
	#if 0
	// init colors
	for(int i = 0; i < Player::MAX_PLAYER + 1; i++)
	{
		unsigned int color = this->m_game->getPlayer(i).getPlayerColor();
		int r = (color & 0xff0000) >> 16;
		int g = (color & 0x00ff00) >> 8;
		int b = (color & 0x0000ff) >> 0;
		
		ncurses::init_color(SC_NCURSES_UNIT_COLOR_BASE+i, (1000*r)/255, (1000*g)/255, (1000*b)/255);
	}
	#endif
	// init color pairs
	for(int i = 0; i < Player::MAX_PLAYER + 1; i++)
	{
		ncurses::init_pair(SC_NCURSES_UNIT_COLOR_PAIR_BASE+i, SC_NCURSES_UNIT_COLOR_BASE+i, COLOR_WHITE);
	}
	
	int x = 0, y = 0;
	//								h	w	y	x
	this->m_wnd_stat = ncurses::newwin(1, 120, y, x);
	y += 1; x = 0;
	
	this->m_wnd_map = ncurses::newwin(25, 120, y, x);
	y += 25; x = 0;
	
	this->m_wnd_minimap = ncurses::newwin(10, 20, y, x); x += 20;
	this->m_wnd_unitstat = ncurses::newwin(10, 80, y, x); x += 80;
	this->m_wnd_btns = ncurses::newwin(10, 20, y, x); x += 20;
	y += 10; x = 0;
	
	fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, NULL) | O_NONBLOCK);
	//ncurses::clear();
	//ncurses::refresh();
	this->m_cur_y=0;this->m_cur_x=0;
	
	return true;
}

bool GameUI_ncurses::cleanupUI()
{
	ncurses::delwin(this->m_wnd_stat);
	ncurses::delwin(this->m_wnd_map);
	ncurses::delwin(this->m_wnd_minimap);
	ncurses::delwin(this->m_wnd_unitstat);
	ncurses::delwin(this->m_wnd_btns);
	
	ncurses::clear();
	#if 0
	ncurses::mvprintw(10, 39, "Quitting...");
	ncurses::refresh();
	sleep(1);
	#endif
	ncurses::endwin();
	
	return true;
}

void GameUI_ncurses::processFrame()
{
	int ch = ncurses::getch();
	if(ch != ERR)
	{
		switch(ch)
		{
		case KEY_UP: this->m_cur_y--; break;
		case KEY_DOWN: this->m_cur_y++; break;
		case KEY_LEFT: this->m_cur_x--; break;
		case KEY_RIGHT: this->m_cur_x++; break;
		case '\n': {
			Coordinate cur1(this->m_cur_x*10, this->m_cur_y*20);
			Coordinate cur2(this->m_cur_x*10 + 9, this->m_cur_y*20 + 19);
			const UnitOrder::OrderPtr &order = this->m_player->getOrder();
			UnitOrder::TargetedOrder *tgorder = dynamic_cast<UnitOrder::TargetedOrder *>(order.get());
			
			// if there's a pending targeted order
			if(tgorder && tgorder->isStarted() == false)
			{
				this->m_player->setOrderTargetByRectCoord(cur1, cur2);
				this->m_player->multiDoCurrentOrder();
			}
			else
			{
				this->m_player->startObjectSelection(cur1);
				this->m_player->finishObjectSelection(cur2);
			}
			}
			break;
		case 'm':
			this->m_player->setOrder(new UnitOrder::Move);
			break;
		case 'a':
			this->m_player->setOrder(new UnitOrder::Attack);
			break;
		}
	}
}

void GameUI_ncurses::draw()
{
	ncurses::wclear(this->m_wnd_stat);
	ncurses::wclear(this->m_wnd_map);
	ncurses::wclear(this->m_wnd_minimap);
	ncurses::wclear(this->m_wnd_unitstat);
	ncurses::wclear(this->m_wnd_btns);
	
	this->drawUI();
	this->drawMap();
	this->drawObjects();
	
	ncurses::wnoutrefresh(this->m_wnd_stat);
	ncurses::wnoutrefresh(this->m_wnd_map);
	ncurses::wnoutrefresh(this->m_wnd_minimap);
	ncurses::wnoutrefresh(this->m_wnd_unitstat);
	ncurses::wnoutrefresh(this->m_wnd_btns);
	ncurses::move(this->m_cur_y+1, this->m_cur_x);
	ncurses::wnoutrefresh(ncurses::stdscr);
	ncurses::doupdate();
}


void GameUI_ncurses::drawUI()
{
	Game *game = this->m_game;
	Player *me = this->m_player;
	
	//ncurses::mvwprintw(this->m_wnd_stat, 0, 0, 
	//	"Minerals: %d | Supplies: %d/%d", 
	//	me->getMinerals(), me->getSuppliesInUse(), me->getCurrentSupplies());
	ncurses::mvwprintw(this->m_wnd_stat, 0, 0, "FPS: %f | Frame: %u | Minerals: %d | Supplies: %d/%d | pos: %dx%d", 
		game->getCurrentFPS(), game->getFrameNumber(), 
		me->getMinerals(), me->getSuppliesInUse(me->getRaceId()), me->getCurrentSupplies(me->getRaceId()), 
		this->m_cur_x*10, this->m_cur_y*20);
	
	{
		const UnitOrder::OrderPtr &order = this->m_player->getOrder();
		if(dynamic_cast<UnitOrder::TargetedOrder *>(order.get()) != NULL && order->isStarted() == false)
		{
			const char *msg = _("Select target");
			ncurses::mvwprintw(this->m_wnd_map, 23, (120 - strlen(msg))/2, "%s", msg);
		}
	}
	if(this->checkToast())
	{
		ncurses::mvwprintw(this->m_wnd_map, 24, (120 - this->m_toast.msg.size())/2, "%s", this->m_toast.msg.data());
	}
	
	//box(this->m_wnd_ctl, 0 , 0);
	box(this->m_wnd_minimap, 0 , 0);
	box(this->m_wnd_unitstat, 0 , 0);
	box(this->m_wnd_btns, 0 , 0);
	
	this->drawUI_MinimapWnd();
	this->drawUI_UnitStatWnd();
	this->drawUI_ButtonsWnd();
}



void GameUI_ncurses::drawUI_MinimapWnd()
{
	
}

void GameUI_ncurses::drawUI_UnitStatWnd()
{
	
}

void GameUI_ncurses::drawUI_ButtonsWnd()
{
	
}

void GameUI_ncurses::drawMap()
{
	
}

const std::string *GameUI_ncurses::getObjectImg(ObjectId_t id) const
{
	const std::map<ObjectId_t, std::string> &obj_imgs = GameUI_ncurses::obj_images;
	std::map<ObjectId_t, std::string>::const_iterator it = obj_imgs.find(id);
	if(it == obj_images.end())
		return NULL;
	else
		return &it->second;
}

void GameUI_ncurses::drawObject(const ObjectPtr &obj)
{
	int cx, cy, w, h;
	int owner_id = obj->getOwner()->getPlayerId();
	obj->getPosition(&cx, &cy);
	obj->getSize(&w, &h);
	int x = cx - w/2, y = cy - h/2;
	x = roundf((float)x/10); w = roundf((float)w/10);
	y = roundf((float)y/20); h = roundf((float)h/20);
	
	const std::string *img_ss = this->getObjectImg(obj->getObjectId());
	if(!img_ss)
		throw new Exception("Object image resource has not been loaded");
	const char *img = img_ss->c_str();
	
	ncurses::wattron(this->m_wnd_map, COLOR_PAIR(SC_NCURSES_UNIT_COLOR_PAIR_BASE+owner_id));
	for(int i = 0; i < h; i++, y++)
	{
		ncurses::mvwprintw(this->m_wnd_map, y, x, "%s", img, owner_id);
		img += strlen(img) + 1;
	}
	ncurses::wattroff(this->m_wnd_map, COLOR_PAIR(SC_NCURSES_UNIT_COLOR_PAIR_BASE+owner_id));
}






#ifndef DRAW_OBJECTS_WITH_VIRTUAL_FXNS
void GameUI_ncurses::drawObjects()
{
	ObjectList &objs = this->m_game->getObjectList();
	
	for(ObjectList::const_iterator it = objs.begin(); it != objs.end(); it++)
	{
		this->drawObject(*it);
	}
}
#endif








#endif /* #ifndef NO_NCURSES_UI */






