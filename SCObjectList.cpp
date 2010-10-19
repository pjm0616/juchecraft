// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details



#include <tr1/memory>
#include <list>

#include "SCCoordinate.h"
#include "SCPlayer.h"
#include "SCObject.h"
#include "SCObjectList.h"

using namespace SC;


Object *ObjectList::addObject(Object *obj)
{
	obj->init();
	this->objects.push_back(std::tr1::shared_ptr<Object>(obj));
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




