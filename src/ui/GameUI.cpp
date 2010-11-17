// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

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
#include "game/ObjectIdList.h"
#include "game/actions/UnitAction.h"
#include "game/orders/UnitOrder.h"
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




