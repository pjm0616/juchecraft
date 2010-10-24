// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUserInterface_SDL_H_
#define SCUserInterface_SDL_H_

namespace SDL
{
	struct SDL_Surface;
	
	struct _TTF_Font;
	typedef _TTF_Font TTF_Font;
}

namespace SC {


class UserInterface_SDL: public UserInterface
{
public:
	// dirpath: "./res/ui/sdl/object/"
	static void load_resources(const char *dirpath);
	
	UserInterface_SDL(Game *game);
	virtual ~UserInterface_SDL();
	
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
	void drawObject(const ObjectSPtr_t &obj);
	void drawObjects();
	#endif
	
private:
	bool isSelectedUnit(const ObjectSPtr_t &obj);
	
	// FIXME: use accessor, mutators
	bool m_selection_in_progress;
	Coordinate m_selection_start_coordinate;
	ObjectList m_selected_objs;
	
	Coordinate m_mouse_pos_in_gamescr;
	
private:
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
	
	int m_gamescr_left_pos, m_gamescr_top_pos;
	
private:
	SDL::SDL_Surface *m_sf_console;
};


} // end of namespace SC

#endif

