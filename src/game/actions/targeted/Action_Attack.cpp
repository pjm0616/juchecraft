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

/** @file src/game/actions/targeted/Action_Attack.cpp
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

#include "game/actions/targeted/Action_Attack.h"

using namespace SC;
using namespace SC::UnitAction;



Attack::Attack(const Target &target)
	:TargetedAction(target, UnitAction::ActionId::Attack)
{
	this->setTarget(target);
}

Attack::~Attack()
{
	this->cleanup();
}



bool Attack::initAction(const ObjectPtr &obj)
{
	if(!this->super::initAction(obj))
		return false;
	if(!this->getTarget().isObjectTarget())
		return false;
	
	// copied from Attack::process, to check if we can attack before calling process(). it's not good. FIXME
	const ObjectPtr &target = this->getTarget().getObject();
	if(!obj->canAttack(target))
		return false;
	// should we do this? hmm. no.
	//if(!obj->checkMinDistance(target, obj->getNetAttackRange(), NULL))
	//	return false;
	
	Game *game = obj->getGame();
	this->setLastAttackTime(game->getCachedElapsedTime()); // FIXME
	this->setAsStarted(true);
	
	return true;
}

void Attack::cleanup()
{
	
}


ProcessResult_t Attack::process(float time)
{
	// if this function is called without being activated, return true (and remove this action in actionlist)
	if(unlikely(this->isFinished()))
		return true;
	ObjectPtr obj = this->getObject();
	Game *game = obj->getGame();
	const ObjectPtr &target = this->getTarget().getObject();
	bool is_finished = false;
	
	// we put these here because unit attrs can change runtime
	if(unlikely(!obj->canAttack(target)))
	{
		is_finished = true;
	}
	else
	{
		Coordinate where_to_move;
		if(obj->checkMinDistance(target, obj->getNetAttackRange(), &where_to_move))
		{
			obj->setAngle(obj->getPosition().calculateAngle(target->getPosition()));
		
			double elapsed_time = game->getCachedElapsedTime();
			float attack_speed = obj->getNetAttackSpeed(); // num_of_attacks per second
			float last_attack = this->getLastAttackTime();
		
			int nattacks = (elapsed_time - last_attack) / (1.0 / attack_speed);
			if(nattacks > 0)
				this->setLastAttackTime(elapsed_time);
			
			for(; nattacks > 0; nattacks--)
			{
				float net_damage = target->hit(obj->getNetDamage());	
				float hp = target->getHP();
				if(hp <= 0)
				{
					game->removeObject(target);
					// FIXME
					//target->kill();
					
					is_finished = true;
					break;
				}
			}
		} /* if(this->checkMinDistance(target, this->getNetAttackRange(), &where_to_move)) */
		else
		{
			// out of range
			is_finished = true;
		}
	}
	
	ProcessResult_t result = ProcessResult::Continue;
	if(is_finished)
	{
		result = ProcessResult::Finished;
		this->setAsFinished(true);
	}
	return result;
}





