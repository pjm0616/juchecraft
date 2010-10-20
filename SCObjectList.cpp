// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details



#include <tr1/memory>
#include <list>
#include <algorithm>

#include "SCCoordinate.h"
#include "SCPlayer.h"
#include "SCObject.h"
#include "SCObjectList.h"

using namespace SC;


Object *ObjectList::addObject(Object *obj)
{
	obj->init();
	this->objects.push_back(std::tr1::shared_ptr<Object>(obj));
	this->setIteratorAsInvalidated();
	return obj;
}

int ObjectList::removeObject(Object *obj)
{
	obj->cleanup();
	int nremoved = 0;
	for(ObjectList::iterator it = this->begin(); 
		it != this->end(); )
	{
		if(it->get() == obj)
		{
			this->objects.erase(it++);
			nremoved++;
			break;
		}
		else
			++it;
	}
	
	this->setIteratorAsInvalidated();
	return nremoved;
}

void ObjectList::removeAllObjects()
{
	for(ObjectList::iterator it = this->begin(); 
		it != this->end(); )
	{
		it->get()->cleanup();
		this->objects.erase(it++);
	}
}


ObjectList::iterator ObjectList::find(Object *obj)
{
	return std::find(this->begin(), this->end(), std::tr1::shared_ptr<Object>(obj));
}

ObjectList::const_iterator ObjectList::find(Object *obj) const
{
	return std::find(this->begin(), this->end(), std::tr1::shared_ptr<Object>(obj));
}






