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





