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

/** @file src/ui/GameUI_SDL.h
**/

#ifndef SCGameUI_SDL_H_
#define SCGameUI_SDL_H_

namespace SDL
{
	struct SDL_Surface;
	
	struct _TTF_Font;
	typedef _TTF_Font TTF_Font;
}

namespace SC {

class GameUI_SDL: public GameUI
{
public:
	// dirpath: "./res/ui/sdl/object/"
	static void load_resources(const char *dirpath);
	
	GameUI_SDL(Game *game, Player *player);
	virtual ~GameUI_SDL();
	
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
	Coordinate m_mouse_pos;
	Coordinate m_mouse_pos_in_gamescr;
	
private:
	bool loadResources();
	
	void drawUI();
	void drawMap();
	void drawUI_MinimapWnd();
	void drawUI_UnitStatWnd();
	void drawUI_ButtonsWnd();
	
	int getFontSize() const { return 14; } // 16
	
	SDL::SDL_Surface *m_screen;
	SDL::TTF_Font *m_font;
	
	SDL::SDL_Surface *m_game_scr;
	SDL::SDL_Surface *m_minimap_wnd, *m_unitstat_wnd, *m_buttons_wnd;
	
	float m_gamescr_left_pos, m_gamescr_top_pos;
};


} // end of namespace SC

#endif

