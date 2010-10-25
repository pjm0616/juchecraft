// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"

#include "smart_ptrs.h"
#include <string>
#include <list>
#include <map>

#include "defs.h"
#include "compat.h"
#include "luacpp/luacpp.h"
#include "SCException.h"
#include "SCCoordinate.h"
#include "SCPlayer.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCObjectPrototypes.h"
#include "SCGame.h"

#include "ui/SCUserInterface.h"


using namespace SC;

#ifdef DRAW_OBJECTS_WITH_VIRTUAL_FXNS
void UserInterface::drawObjects()
{
	ObjectList &objs = this->m_game->getObjectList();
	
	for(ObjectList::const_iterator it = objs.begin(); it != objs.end(); it++)
	{
		this->drawObject(*it);
	}
}
#endif




