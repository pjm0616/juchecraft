// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"

#include "smart_ptrs.h"
#include <list>
#include <algorithm>

#include "defs.h"
#include "SCCoordinate.h"
#include "SCPlayer.h"
#include "SCObject.h"
#include "SCObjectList.h"

using namespace SC;


Object *ObjectList::addObject(Object *obj)
{
	this->getObjects().push_back(ObjectList::objptr_t(obj));
	this->setIteratorAsInvalidated();
	return obj;
}

int ObjectList::removeObject(Object *obj)
{
	ObjectList::objlist_t &objs = this->getObjects();
	int nremoved = 0;
	for(ObjectList::iterator it = this->begin(); 
		it != this->end(); )
	{
		if(it->get() == obj)
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



ObjectList::iterator ObjectList::find(Object *obj)
{
	return std::find(this->begin(), this->end(), ObjectList::objptr_t(obj));
}

ObjectList::const_iterator ObjectList::find(Object *obj) const
{
	return std::find(this->begin(), this->end(), ObjectList::objptr_t(obj));
}






