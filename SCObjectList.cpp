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
#ifdef DEBUG
# include <iostream>
#endif

#include "defs.h"
#include "SCTypes.h"
#include "SCCoordinate.h"
#include "SCObjectIdList.h"
#include "SCUnitAction.h"
#include "SCUnitCommand.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCPlayer.h"

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
#ifdef DEBUG
	for(ObjectList::iterator it = this->m_objects.begin(); 
		it != this->m_objects.end(); )
	{
		if(it->use_count() != 1)
		{
			std::cout << "Warning: ObjectList::clear: " << it->get()->getObjectName() << ": " << it->use_count() <<std::endl;
		}
		this->m_objects.erase(it++);
	}
#else
	this->getObjects().clear();
#endif
}





