// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"

#include "smart_ptrs.h"
#include <list>
#include <string>
#include <algorithm>
#include <map>

#include "defs.h"
#include "SCCoordinate.h"
#include "SCPlayer.h"
#include "SCObject.h"
#include "SCObjectList.h"

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



ObjectList::iterator ObjectList::find(const ObjectSPtr_t &obj)
{
	return std::find(this->begin(), this->end(), obj);
}

ObjectList::const_iterator ObjectList::find(const ObjectSPtr_t &obj) const
{
	return std::find(this->begin(), this->end(), obj);
}






