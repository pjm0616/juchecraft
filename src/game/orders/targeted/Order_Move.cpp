/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/orders/targeted/Order_Move.cpp
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
#include "game/Object.h"
#include "game/ObjectList.h"
#include "game/ObjectFactory.h"
#include "game/Player.h"
#include "game/Game.h"

#include "game/orders/targeted/Order_Move.h"

using namespace SC;
using namespace SC::UnitOrder;


Move::Move()
	:TargetedOrder(OrderId::Move)
{
	this->m_state.step = 0;
}
Move::Move(const Target &target)
	:TargetedOrder(target, OrderId::Move)
{
	this->m_state.step = 0;
}

OrderPtr Move::clone(OrderPtr cloned_order)
{
	Move *p = this->do_clone_head<Move, TargetedOrder>(cloned_order);
	
	return cloned_order;
}



bool Move::initOrder(const ObjectPtr &obj)
{
	return this->TargetedOrder::initOrder(obj);
}

bool Move::process(float time)
{
	bool result = true;
	switch(this->m_state.step)
	{
	case 0: {
		bool ret = this->getObject()->doAction(new UnitAction::Move(this->getTarget()));
		if(!ret)
		{
			result = false;
			this->getObject()->getOwner()->toast(_("Unable to move"));
		}
		else
		{
			this->m_state.step = 1;
		}
		break; }
	case 1: {
		if(!this->getObject()->isActivatedAction(UnitAction::ActionId::Move))
		{
			result = false;
		}
		break; }
	}
	
	return result;
}









