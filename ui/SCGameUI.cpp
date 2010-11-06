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
#include "SCTypes.h"
#include "SCException.h"
#include "SCCoordinate.h"
#include "SCObjectIdList.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCObjectFactory.h"
#include "SCUnitCommand.h"
#include "SCPlayer.h"
#include "SCGame.h"

#include "ui/SCGameUI.h"


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




