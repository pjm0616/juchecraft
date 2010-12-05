/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/orders/TargetedOrder.cpp
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
using namespace SC::UnitOrder;



TargetedOrder::TargetedOrder(OrderId_t orderid)
{
}

TargetedOrder::TargetedOrder(const Target &target, OrderId_t orderid)
{
	this->setTarget(target);
}

TargetedOrder::~TargetedOrder()
{
}

bool TargetedOrder::initOrder(const ObjectPtr &obj)
{
	return this->Order::initOrder(obj);
}

ProcessResult_t TargetedOrder::process(float deltat)
{
	return this->Order::process(deltat);
}

OrderPtr TargetedOrder::clone(OrderPtr cloned_order)
{
	TargetedOrder *p = this->do_clone_head<TargetedOrder, Order>(cloned_order);
	
	p->m_target = this->m_target;
	
	return cloned_order;
}




