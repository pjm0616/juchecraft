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
#include "game/orders/Attack.h"
#include "game/Object.h"
#include "game/ObjectList.h"
#include "game/ObjectFactory.h"
#include "game/Player.h"
#include "game/Game.h"

using namespace SC;
using namespace SC::UnitOrder;



OrderPtr Attack::clone(OrderPtr cloned_order)
{
	Attack *p = this->do_clone_head<Attack, TargetedOrder>(cloned_order);
	
	return cloned_order;
}


bool Attack::initOrder(const ObjectPtr &obj)
{
	return this->TargetedOrder::initOrder(obj);
}

bool Attack::process(float time)
{
	bool result = true;
	const ObjectPtr &obj = this->getObject();
	const Target &target = this->getTarget();
	Player *player = obj->getOwner();
	
	switch(this->m_state.step)
	{
	case 0: { // start attack
		if(this->getTarget().isObjectTarget())
		{
			if(!obj->doAction(new UnitAction::Attack(target.getObject())))
			{
				player->toast(_("Unable to attack target"));
				result = false;
			}
		
			this->m_state.step = 1;
		}
		else
		{
			if(!obj->doAction(new UnitAction::Move(target, 
				UnitAction::Move::MovementFlags::AutomaticallyAttack)))
			{
				this->m_state.step = 100;
			}
		}
		
		break; }
	case 100: { // target is a coordinate
		if(!obj->isActivatedAction(UnitAction::ActionId::Move))
		{
			result = false;
		}
		else
		{
			// TODO: automatically attack if there's an enemy.
		}
		break; }
	case 1: { // attack
		if(!obj->isActivatedAction(UnitAction::ActionId::Attack))
		{
			if(!obj->canAttack(target.getObject()))
			{
				result = false;
			}
			else
			{
				// probably out of range
				bool ret = obj->doAction(new UnitAction::Move(target, obj->getNetAttackRange()));
				if(!ret)
				{
					player->toast(_("Unable to reach target"));
					result = false;
				}
				else
				{
					this->m_state.step = 2;
				}
			}
		}
		break; }
	case 2: { // move to reach target
		if(!obj->isActivatedAction(UnitAction::ActionId::Move))
		{
			if(obj->canAttack(target.getObject()))
			{
				this->m_state.step = 0;
			}
			else
			{
				result = false;
			}
		}
		break; }
	}
	
	return result;
}









