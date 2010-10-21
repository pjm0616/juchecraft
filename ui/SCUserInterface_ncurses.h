// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUserInterface_ncurses_H_
#define SCUserInterface_ncurses_H_

// damn..
namespace ncurses
{
#include <ncurses.h>
}

namespace SC {


class UserInterface_ncurses: public UserInterface
{
public:
	// dirpath: "./res/object/"
	static void load_resources(const char *dirpath);
	
	UserInterface_ncurses(Game *game);
	virtual ~UserInterface_ncurses();
	
	virtual void processFrame();
	virtual void draw();
	
protected:
	static std::map<ObjectId_t, std::string> obj_images;
	static bool is_initialized;
	
	const std::string *getObjectImg(ObjectId_t id) const;
	virtual void drawObject(Object &obj);
	
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

#endif

