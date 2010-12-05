/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/orders/ProductionOrder.cpp
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

using namespace SC;
using namespace SC::UnitOrder;



ProductionOrder::ProductionOrder()
{
}

ProductionOrder::ProductionOrder(const ProductionInfoPtr &info)
{
	this->m_prodinfo = info;
}

ProductionOrder::~ProductionOrder()
{
}

bool ProductionOrder::initOrder(const ObjectPtr &obj)
{
	return this->Order::initOrder(obj);
}

bool ProductionOrder::process(float deltat)
{
	return this->Order::process(deltat);
}

OrderPtr ProductionOrder::clone(OrderPtr cloned_order)
{
	ProductionOrder *p = this->do_clone_head<ProductionOrder, Order>(cloned_order);
	
	p->m_prodinfo = this->m_prodinfo;
	
	return cloned_order;
}




