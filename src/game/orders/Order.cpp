/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/orders/Order.cpp
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


Order::Order(OrderId_t orderid)
	: m_orderid(orderid)
	, m_rank(0) // defaults to primary order
{
	this->setAsFinished(false);
	this->setAsStarted(false);
}

Order::~Order()
{
}

bool Order::initOrder(const ObjectPtr &obj)
{
	this->m_obj = obj;
	this->setAsStarted(true);
	return true;
}

ProcessResult_t Order::process(float deltat)
{
	return ProcessResult::Finished;
}

OrderPtr Order::clone(OrderPtr cloned_order)
{
	if(!cloned_order)
		cloned_order.reset(new Order);
	
	Order *p = cloned_order.get();
	p->m_orderid = this->m_orderid;
	p->m_obj = this->m_obj;
	p->m_info = this->m_info;
	p->m_rank = this->m_rank;
	
	return cloned_order;
}





