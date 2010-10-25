// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

/** @file ui/SCGameUI.h
 *  @brief Abstract user interface class
**/


#ifndef SCGameUI_H_
#define SCGameUI_H_

namespace SC {

//#define DRAW_OBJECTS_WITH_VIRTUAL_FXNS

class GameUI
{
public:
	GameUI(Game *game, const PlayerSPtr_t &player)
		:m_game(game), 
		m_player(player)
	{
		this->setFPS(20);
	}
	virtual ~GameUI() {}
	
	virtual bool initUI() = 0;
	virtual bool cleanupUI() = 0;
	
	/** @brief 1초동안 화면을 다시 그리는 횟수
	 *  @detail 너무 높으면 메인 루프가 지연됨. 메인 FPS제한 푼 상태에서도 RedrawFPS가 30을 넘으면 FPS가 50을 못넘음
	 */
	inline unsigned int getFPS() const { return this->m_fps; }
	
	virtual void processFrame() = 0;
	virtual void draw() = 0;
	
	// Uncomment this if you need a public accessor for m_game.
	//Game *getGame() { return this->m_game; }
	// Uncomment this if you need a public accessor for m_player.
	//const PlayerSPtr_t &getPlayer() const { return this->m_player; }
	
protected:
	void setFPS(unsigned int fps) { this->m_fps = fps; }
	
	#ifdef DRAW_OBJECTS_WITH_VIRTUAL_FXNS
	virtual void drawObject(const ObjectSPtr_t &obj) = 0;
	void drawObjects();
	#else
	void drawObject(Object &obj) { throw SC::Exception("BUG!!! w8cmoaeyr8myncafo"); }
	void drawObjects() { throw SC::Exception("BUG!!! a3djyo943pe"); }
	#endif
	
	Game *m_game;
	PlayerSPtr_t m_player;
	unsigned m_fps;
};


} // end of namespace SC

#endif

