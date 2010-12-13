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

/** @file src/ui/GameUI.cpp
**/

#include "config.h"

#include "smart_ptrs.h"
#include <string>
#include <list>
#include <map>
#include <vector>
#include <deque>

#include "defs.h"
#include "compat.h"
#include "luacpp/luacpp.h"
#include "game/Types.h"
#include "game/Exception.h"
#include "game/Coordinate.h"
#include "game/Target.h"
#include "game/ObjectIdList.h"
#include "game/actions/Actions.h"
#include "game/orders/Orders.h"
#include "game/UnitProductionManager.h"
#include "game/Object.h"
#include "game/ObjectList.h"
#include "game/ObjectFactory.h"
#include "game/Player.h"
#include "game/Game.h"

#include "ui/GameUI.h"


using namespace SC;

#ifdef DRAW_OBJECTS_WITH_VIRTUAL_FXNS
void GameUI::drawObjects()
{
	ObjectList &objs = this->m_game->getObjectList();
	
	for(ObjectList::const_iterator it = objs.begin(); it != objs.end(); it++)
	{
		this->drawObject(*it);
	}
}
#endif



void GameUI::toast(const std::string &msg, time_t duration)
{
	this->m_toast.msg = msg;
	this->m_toast.duration = duration;
	this->m_toast.started = this->m_game->getCachedElapsedTime();
}
void GameUI::clearToast()
{
	this->m_toast.msg.clear();
	this->m_toast.duration = 0;
	this->m_toast.started = 0;
}
bool GameUI::checkToast()
{
	if(this->m_toast.msg.empty())
	{
		return false;
	}
	else if((this->m_game->getCachedElapsedTime() - this->m_toast.started) >= this->m_toast.duration)
	{
		this->clearToast();
		return false;
	}
	else
	{
		return true;
	}
}








