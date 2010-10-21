// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details



#include <tr1/memory>
#include <string>
#include <list>
#include <map>

#include "SCException.h"
#include "SCCoordinate.h"
#include "SCPlayer.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCGame.h"

#include "ui/SCUserInterface.h"


using namespace SC;

void UserInterface::drawObjects()
{
	ObjectList &objs = this->m_game->getObjectList();
	
	#if 0
	for(ObjectList::const_iterator it = objs.begin(); it != objs.end(); it++)
	{
		this->drawObject(*it->get());
	}
	#else
	for(ObjectList::const_reverse_iterator it = objs.rbegin(); it != objs.rend(); it++)
	{
		this->drawObject(*it->get());
	}
	#endif
}




