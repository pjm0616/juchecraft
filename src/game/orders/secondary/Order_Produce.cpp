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

/** @file src/game/orders/Order_Produce.cpp
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



Produce::Produce(ProductionInfoPtr info)
	: Order(OrderId::Move)
{
	this->m_prodinfo = info;
	this->setAsSecondaryOrder();
}

Produce::~Produce()
{
}

bool Produce::initOrder(const ObjectPtr &obj)
{
	if(!this->super::initOrder(obj))
		return false;
	
	obj->m_unit_producer.addToProductionQueue(this->m_prodinfo);
	return true;
}

ProcessResult_t Produce::process(float time)
{
	return ProcessResult::Finished;
}

OrderPtr Produce::clone(OrderPtr cloned_order)
{
	Produce *p = this->do_clone_head<Produce, Order>(cloned_order);
	
	p->m_prodinfo = this->m_prodinfo->clone();
	
	return cloned_order;
}


