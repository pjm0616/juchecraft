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
#include "game/actions/UnitActionList.h"
#include "game/orders/UnitOrderList.h"
#include "game/orders/Move.h"
#include "game/Object.h"
#include "game/ObjectList.h"
#include "game/ObjectFactory.h"
#include "game/Player.h"
#include "game/Game.h"

using namespace SC;
using namespace SC::UnitOrder;

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
	bool res = true;
	switch(this->m_state.step)
	{
	case 0:
		this->getObject()->doAction(new UnitAction::Move(this->getTarget()));
		this->m_state.step++;
		break;
	}
	
	return res;
}









