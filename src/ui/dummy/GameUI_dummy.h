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

/** @file src/ui/GameUI_dummy.h
**/

#ifndef SCGameUI_dummy_H_
#define SCGameUI_dummy_H_


namespace SC {


class GameUI_dummy: public GameUI
{
public:
	GameUI_dummy(Game *game, Player *player)
		:GameUI(game, player)
	{
	}
	virtual ~GameUI_dummy() {}
	
	virtual bool initUI() { return true; }
	virtual bool cleanupUI() { return true; }
	
	virtual void processFrame()
	{
		if(this->m_game->getElapsedTime() > 10.0)
			this->m_game->endGame();
	}
	virtual void draw() {}
	
	virtual void toast(const std::string &msg, time_t duration = GameUI::Toast::DefaultDuration) {}
};


} // end of namespace SC

#endif

