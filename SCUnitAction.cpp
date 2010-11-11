// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"

#include "smart_ptrs.h"
#include <string>
#include <list>
#include <map>
#include <vector>
#include <deque>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cassert>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "defs.h"
#include "compat.h"
#include "luacpp/luacpp.h"
#include "SCTypes.h"
#include "SCException.h"
#include "SCCoordinate.h"
#include "SCObjectIdList.h"
#include "SCUnitAction.h"
#include "SCUnitCommand.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCObjectFactory.h"
#include "SCPlayer.h"
#include "SCGame.h"

using namespace SC;




UnitAction::UnitAction(UnitActionId_t actid)
	: m_actid(actid)
	, m_is_activated(false)
{
}
UnitAction::UnitAction(const ObjectPtr &obj, UnitActionId_t actid)
	: m_actid(actid)
	, m_is_activated(false)
{
	this->setObject(obj);
}
UnitAction::~UnitAction()
{
}
UnitActionPtr UnitAction::makeThisPtr()
{
	assert(this->m_this.expired() == true);
	UnitActionPtr ptr(this);
	this->m_this = ptr;
	return ptr;
}

void UnitAction::setObject(const ObjectPtr &obj)
{
	// if this->m_obj is already been set, throw an exception
	//assert(!this->m_obj);
	
	this->m_obj = obj;
}



