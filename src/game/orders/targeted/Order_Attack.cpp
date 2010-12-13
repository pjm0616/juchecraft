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

/** @file src/game/orders/targeted/Order_Attack.cpp
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

#include "game/orders/targeted/Order_Attack.h"

using namespace SC;
using namespace SC::UnitOrder;


Attack::Attack()
	:TargetedOrder(OrderId::Attack)
{
	this->m_state.step = 0;
}
Attack::Attack(const Target &target)
	:TargetedOrder(target, OrderId::Attack)
{
	this->m_state.step = 0;
}

OrderPtr Attack::clone(OrderPtr cloned_order)
{
	Attack *p = this->do_clone_head<Attack, super>(cloned_order);
	
	return cloned_order;
}


bool Attack::initOrder(const ObjectPtr &obj)
{
	return this->super::initOrder(obj);
}

ProcessResult_t Attack::process(float time)
{
	ProcessResult_t result = ProcessResult::Continue;
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
				result = ProcessResult::Finished;
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
			result = ProcessResult::Finished;
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
				result = ProcessResult::Finished;
			}
			else
			{
				// probably out of range
				bool ret = obj->doAction(new UnitAction::Move(target, obj->getNetAttackRange()));
				if(!ret)
				{
					player->toast(_("Unable to reach target"));
					result = ProcessResult::Finished;
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
				result = ProcessResult::Finished;
			}
		}
		break; }
	}
	
	return result;
}









