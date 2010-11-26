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
#include "game/actions/UnitAction.h"
#include "game/orders/UnitOrder.h"
#include "game/Object.h"
#include "game/ObjectList.h"
#include "game/ObjectFactory.h"
#include "game/Player.h"
#include "game/Game.h"

using namespace SC;
using namespace SC::UnitOrder;


Order::Order(OrderId_t orderid)
	: m_orderid(orderid)
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

bool Order::process(float deltat)
{
	return true;
}

OrderPtr Order::clone(OrderPtr cloned_order)
{
	if(!cloned_order)
		cloned_order.reset(new Order);
	
	Order *p = cloned_order.get();
	p->m_orderid = this->m_orderid;
	p->m_obj = this->m_obj;
	p->m_info = this->m_info;
	
	return cloned_order;
}



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

bool TargetedOrder::process(float deltat)
{
	return this->Order::process(deltat);
}

OrderPtr TargetedOrder::clone(OrderPtr cloned_order)
{
	TargetedOrder *p = this->do_clone_head<TargetedOrder, Order>(cloned_order);
	
	p->m_target = this->m_target;
	
	return cloned_order;
}




