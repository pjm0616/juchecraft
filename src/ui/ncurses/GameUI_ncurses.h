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

/** @file src/ui/GameUI_ncurses.h
**/

#ifndef SCGameUI_ncurses_H_
#define SCGameUI_ncurses_H_

#ifndef SC_CONFIG_H_
#error Include "config.h" first
#endif

#ifndef NO_NCURSES_UI

// damn..
namespace ncurses
{
#include <ncurses.h>
}

namespace SC {


class GameUI_ncurses: public GameUI
{
public:
	// dirpath: "./res/object/"
	void load_resources(const char *dirpath);
	
	GameUI_ncurses(Game *game, Player *player);
	virtual ~GameUI_ncurses();
	
	virtual bool initUI();
	virtual bool cleanupUI();
	
	virtual void processFrame();
	virtual void draw();
	
protected:
	static std::map<ObjectId_t, std::string> obj_images;
	static bool is_initialized;
	
	const std::string *getObjectImg(ObjectId_t id) const;
	#ifdef DRAW_OBJECTS_WITH_VIRTUAL_FXNS
	virtual void drawObject(Object *obj);
	#else
	void drawObject(const ObjectPtr &obj);
	void drawObjects();
	#endif
	
private:
	void drawUI();
	void drawMap();
	void drawUI_MinimapWnd();
	void drawUI_UnitStatWnd();
	void drawUI_ButtonsWnd();
	
	int m_win_x, m_win_y;
	bool m_has_mouse;
	ncurses::WINDOW *m_wnd_stat, *m_wnd_map, *m_wnd_minimap, *m_wnd_unitstat, *m_wnd_btns;
	
	int m_cur_x, m_cur_y;
};


} // end of namespace SC

#endif /* #ifndef NO_NCURSES_UI */

#endif

