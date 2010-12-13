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

/** @file src/game/ObjectList.cpp
**/

#include "config.h"

#include "smart_ptrs.h"
#include <list>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <deque>
#ifdef DEBUG
# include <iostream>
#endif

#include "defs.h"
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
#include "game/Player.h"

using namespace SC;


const ObjectPtr &ObjectList::addObject(const ObjectPtr &obj)
{
	this->getObjects().push_back(obj);
	this->setIteratorAsInvalidated();
	return obj;
}

int ObjectList::removeObject(const ObjectPtr &obj)
{
	ObjectList::objlist_t &objs = this->getObjects();
	int nremoved = 0;
	for(ObjectList::iterator it = this->begin(); 
		it != this->end(); )
	{
		if(*it == obj)
		{
			objs.erase(it++);
			nremoved++;
			break;
		}
		else
		{
			++it;
		}
	}
	
	this->setIteratorAsInvalidated();
	return nremoved;
}

void ObjectList::erase(iterator it)
{
	this->getObjects().erase(it);
	this->setIteratorAsInvalidated();
}


ObjectList::iterator ObjectList::find(const ObjectPtr &obj)
{
	return std::find(this->begin(), this->end(), obj);
}

ObjectList::const_iterator ObjectList::find(const ObjectPtr &obj) const
{
	return std::find(this->begin(), this->end(), obj);
}

void ObjectList::clear()
{
	this->getObjects().clear();
}


bool ObjectList::testCollision(const ObjectPtr &obj)
{
	// TODO: implement this
	
	return false;
}

bool ObjectList::detectCollision(const ObjectPtr &obj)
{
	// TODO: implement this
	
	return false;
}










