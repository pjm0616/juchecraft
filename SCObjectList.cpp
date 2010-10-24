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


const ObjectSPtr_t &ObjectSList::addObject(const ObjectSPtr_t &obj)
{
	this->getObjects().push_back(obj);
	this->setIteratorAsInvalidated();
	return obj;
}

int ObjectSList::removeObject(const ObjectSPtr_t &obj)
{
	ObjectSList::objlist_t &objs = this->getObjects();
	int nremoved = 0;
	for(ObjectSList::iterator it = this->begin(); 
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



ObjectSList::iterator ObjectSList::find(const ObjectSPtr_t &obj)
{
	return std::find(this->begin(), this->end(), obj);
}

ObjectSList::const_iterator ObjectSList::find(const ObjectSPtr_t &obj) const
{
	return std::find(this->begin(), this->end(), obj);
}






