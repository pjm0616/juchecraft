// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

/** @file ui/SCUserInterface.h
 *  @brief Abstract user interface class
**/


#ifndef SCUserInterface_H_
#define SCUserInterface_H_

namespace SC {

//#define DRAW_OBJECTS_WITH_VIRTUAL_FXNS

class UserInterface
{
public:
	UserInterface(Game *game)
		:m_game(game)
	{
		this->setRedrawFPS(this->m_game->getFPS() / 3);
	}
	virtual ~UserInterface() {}
	
	virtual bool initUI() = 0;
	virtual bool cleanupUI() = 0;
	
	/** @brief 1초동안 화면을 다시 그리는 횟수
	 *  @detail 너무 높으면 메인 루프가 지연됨. 메인 FPS제한 푼 상태에서도 RedrawFPS가 30을 넘으면 FPS가 50을 못넘음
	 */
	inline unsigned int getRedrawFPS() const { return this->m_redraw_fps; }
	
	virtual void processFrame() = 0;
	virtual void draw() = 0;
	
	// There's no point having an accessor for m_game.
	//Game *getGame() { return this->m_game; }
	
protected:
	void setRedrawFPS(unsigned int fps) { this->m_redraw_fps = fps; }
	
	#ifdef DRAW_OBJECTS_WITH_VIRTUAL_FXNS
	virtual void drawObject(Object &obj) = 0;
	void drawObjects();
	#else
	void drawObject(Object &obj) { throw SC::Exception("BUG!!! w8cmoaeyr8myncafo"); }
	void drawObjects() { throw SC::Exception("BUG!!! a3djyo943pe"); }
	#endif
	
	Game *m_game;
	unsigned m_redraw_fps;
};


} // end of namespace SC

#endif

