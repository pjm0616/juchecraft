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

/** @file src/ui/GameUI.h
**/

#ifndef SCGameUI_H_
#define SCGameUI_H_

namespace SC {

//#define DRAW_OBJECTS_WITH_VIRTUAL_FXNS

class GameUI
{
public:
	GameUI(Game *game, Player *player)
		: m_game(game)
		, m_player(player)
	{
		this->setFPS(20);
	}
	virtual ~GameUI() {}
	
	virtual bool initUI() = 0;
	/** WARNING: Don't access m_player in this function.
	 */
	virtual bool cleanupUI() = 0;
	
	/** @brief get fps value
	 *  @details if m_fps is too high, main loop can be delayed
	 */
	inline unsigned int getFPS() const { return this->m_fps; }
	
	virtual void processFrame() = 0;
	virtual void draw() = 0;
	
	/** @name Toast
	 */
	//@{
	struct Toast
	{
		enum
		{
			DefaultDuration = 3, 
		};
	};
	virtual void toast(const std::string &msg, time_t duration = GameUI::Toast::DefaultDuration);
	virtual void clearToast();
	//@}
	
	// Uncomment this if you need a public accessor for m_game.
	//Game *getGame() { return this->m_game; }
	// Uncomment this if you need a public accessor for m_player.
	//Player *getPlayer() const { return this->m_player; }
	
protected:
	void setFPS(unsigned int fps) { this->m_fps = fps; }
	
	#ifdef DRAW_OBJECTS_WITH_VIRTUAL_FXNS
	virtual void drawObject(const ObjectPtr &obj) = 0;
	void drawObjects();
	#else
	void drawObject(Object &obj) { throw SC::Exception("BUG!!! w8cmoaeyr8myncafo"); }
	void drawObjects() { throw SC::Exception("BUG!!! a3djyo943pe"); }
	#endif
	
	Game *m_game;
	Player *m_player;
	unsigned m_fps;
	
	/** @name Toast
	 */
	//@{
	virtual bool checkToast();
	struct
	{
		std::string msg;
		time_t duration;
		time_t started;
	} m_toast;
	//@}
};


} // end of namespace SC

#endif

