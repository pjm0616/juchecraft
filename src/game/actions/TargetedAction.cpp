/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/actions/TargetedAction.cpp
**/

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

#ifdef DEBUG
#include <stdio.h>
#endif

#include "defs.h"
#include "compat.h"
#include "luacpp/luacpp.h"
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
#include "game/ObjectFactory.h"
#include "game/Player.h"
#include "game/Game.h"

using namespace SC;
using namespace SC::UnitAction;




TargetedAction::TargetedAction(ActionId_t actid)
	: Action(actid)
{
}
TargetedAction::TargetedAction(const Target &target, ActionId_t actid)
	: Action(actid), m_target(target)
{
}

TargetedAction::~TargetedAction()
{
}

bool TargetedAction::setTarget(const Target &target)
{
	if(target.isObjectTarget())
	{
		if(target.getObject() == this->getObject())
			return false;
	}
	return true;
}

bool TargetedAction::initAction(const ObjectPtr &obj)
{
	if(!this->super::initAction(obj))
		return false;
	
	const Target &target = this->getTarget();
	if(target.isObjectTarget() && target.getObject() == obj)
		return false; // target cannot be itself
	
	return true;
}






