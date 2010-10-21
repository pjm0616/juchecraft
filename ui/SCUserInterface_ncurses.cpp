// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details



#include <tr1/memory>
#include <string>
#include <list>
#include <map>

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

#include "SCException.h"
#include "SCCoordinate.h"
#include "SCPlayer.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCGame.h"

#include "ui/SCUserInterface.h"
#include "ui/SCUserInterface_ncurses.h"

using namespace ncurses; // DAMN....  COLOR_PAIR macro requires this
using namespace SC;

std::map<ObjectId_t, std::string> UserInterface_ncurses::obj_images;
bool UserInterface_ncurses::is_initialized = false;


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
void UserInterface_ncurses::load_resources(const char *dirpath)
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
			readfile(filepath.c_str(), UserInterface_ncurses::obj_images[id]);
			replace_char(UserInterface_ncurses::obj_images[id], '\n', '\0');
		}
	}
	closedir(dp);
	UserInterface_ncurses::is_initialized = true;
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


UserInterface_ncurses::UserInterface_ncurses(Game *game)
	:UserInterface(game)
{
	int ret;
	
	this->setUIFPS(10);
	
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
		unsigned int color = Player::Players[i].getPlayerColor();
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
	this->m_cur_y=10;this->m_cur_x=10;
}

UserInterface_ncurses::~UserInterface_ncurses()
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
}

void UserInterface_ncurses::processFrame()
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
				SC::ObjectList::iterator it = this->m_game->getObjectList().begin();
				it->get()->cmd_move(Coordinate(this->m_cur_x * 10, (this->m_cur_y-1) * 20));
			}
			break;
		case 'a': {
				SC::ObjectList::iterator it = this->m_game->getObjectList().begin();
				SC::ObjectList::iterator it2 = this->m_game->getObjectList().begin(); ++it2;
				
				it->get()->cmd_attack(it2->get());
			}
			break;
		}
	}
}

void UserInterface_ncurses::draw()
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
	ncurses::move(this->m_cur_y, this->m_cur_x);
	ncurses::wnoutrefresh(ncurses::stdscr);
	ncurses::doupdate();
}


void UserInterface_ncurses::drawUI()
{
	Game *game = this->getGame();
	Player *me = &Player::Players[1];
	
	//ncurses::mvwprintw(this->m_wnd_stat, 0, 0, 
	//	"Minerals: %d | Supplies: %d/%d", 
	//	me->getMinerals(), me->getFoodCrnt(), me->getFoodMax());
	ncurses::mvwprintw(this->m_wnd_stat, 0, 0, "FPS: %f | Frame: %u | Minerals: %d | Supplies: %d/%d", 
		game->getCurrentFPS(), game->getFrameNumber(), 
		me->getMinerals(), me->getFoodCrnt(me->getRaceId()), me->getFoodMax(me->getRaceId()));
	
	
	//box(this->m_wnd_ctl, 0 , 0);
	box(this->m_wnd_minimap, 0 , 0);
	box(this->m_wnd_unitstat, 0 , 0);
	box(this->m_wnd_btns, 0 , 0);
	
	this->drawUI_MinimapWnd();
	this->drawUI_UnitStatWnd();
	this->drawUI_ButtonsWnd();
}



void UserInterface_ncurses::drawUI_MinimapWnd()
{
	
}

void UserInterface_ncurses::drawUI_UnitStatWnd()
{
	
}

void UserInterface_ncurses::drawUI_ButtonsWnd()
{
	
}

void UserInterface_ncurses::drawMap()
{
	
}

const std::string *UserInterface_ncurses::getObjectImg(ObjectId_t id) const
{
	const std::map<ObjectId_t, std::string> &obj_imgs = UserInterface_ncurses::obj_images;
	std::map<ObjectId_t, std::string>::const_iterator it = obj_imgs.find(id);
	if(it == obj_images.end())
		return NULL;
	else
		return &it->second;
}

void UserInterface_ncurses::drawObject(Object &obj)
{
	int x, y, w, h;
	int owner_id = obj.getOwner()->getPlayerId();
	obj.getPosition(&x, &y);
	obj.getSize(&w, &h);
	x = roundf((float)x/10); w = roundf((float)w/10);
	y = roundf((float)y/20); h = roundf((float)h/20);
	
	const std::string *img_ss = this->getObjectImg(obj.getObjectId());
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


















