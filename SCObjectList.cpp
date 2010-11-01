// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"

#include "smart_ptrs.h"
#include <list>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <deque>

#include "defs.h"
#include "SCTypes.h"
#include "SCCoordinate.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCUnitCommand.h"
#include "SCPlayer.h"

using namespace SC;


const ObjectSPtr_t &ObjectList::addObject(const ObjectSPtr_t &obj)
{
	this->getObjects().push_back(obj);
	this->setIteratorAsInvalidated();
	return obj;
}

int ObjectList::removeObject(const ObjectSPtr_t &obj)
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


ObjectList::iterator ObjectList::find(const ObjectSPtr_t &obj)
{
	return std::find(this->begin(), this->end(), obj);
}

ObjectList::const_iterator ObjectList::find(const ObjectSPtr_t &obj) const
{
	return std::find(this->begin(), this->end(), obj);
}






