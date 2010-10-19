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
	
	virtual unsigned int getUIFPS() const { return 10; }
	
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
	
	int win_x, win_y;
	bool has_mouse;
	ncurses::WINDOW *wnd_stat, *wnd_map, *wnd_minimap, *wnd_unitstat, *wnd_btns;
	
	int cur_x, cur_y;
};


} // end of namespace SC

#endif

