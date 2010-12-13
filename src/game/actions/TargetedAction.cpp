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






