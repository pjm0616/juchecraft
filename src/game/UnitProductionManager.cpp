/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/Object.cpp
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
#include <iostream>
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




UnitProductionManager::UnitProductionManager(Object *owner)
	: m_obj(owner)
{
}
UnitProductionManager::~UnitProductionManager()
{
}


void UnitProductionManager::clearProductionQueue()
{
	this->m_prodqueue.clear();
}
void UnitProductionManager::addToProductionQueue(const ProductionInfoPtr &what)
{
	this->m_prodqueue.push_back(what);
}
void UnitProductionManager::popFirstSlotInProductionQueue()
{
	this->m_prodqueue.pop_front();
}

void UnitProductionManager::process()
{
	if(this->isQueueEmpty())
		return;
	const ProductionInfoPtr &prodinfo = this->firstSlotInProductionQueue();
	double now = this->getObject()->getGame()->getCachedElapsedTime();
	
	if(!prodinfo->isStarted())
	{
		prodinfo->setStartedTime(now);
	}
	
	if(now - prodinfo->m_state.started_time >= prodinfo->m_time)
	{
		this->do_produce(prodinfo);
		this->popFirstSlotInProductionQueue();
	}
}


void UnitProductionManager::do_produce(const ProductionInfoPtr &prodinfo)
{
	Object *me = this->getObject();
	Game *game = me->getGame();
	Player *player = me->getOwner();
	Coordinate new_pos = me->getPosition();
	
	const ObjectPtr &o = game->newObject(prodinfo->m_objid);
	o->changeOwner(player);
	o->setPosition(new_pos);
}



#if 0

bool Produce::initAction(const ObjectPtr &obj)
{
	if(!this->super::initAction(obj))
		return false;
	
	this->m_prodinfo->m_state.started_time = obj->getGame()->getCachedElapsedTime();
	
	return true;
}

ProcessResult_t Produce::process(float deltat)
{
	ProcessResult_t result = ProcessResult::Continue;
	
	double now = this->getObject()->getGame()->getCachedElapsedTime();
	if(now - this->m_prodinfo->m_state.started_time >= this->m_prodinfo->m_time)
	{
		this->do_produce();
		result = ProcessResult::Finished;
	}
	
	if(result == ProcessResult::Finished)
		this->setAsFinished(true);
	return result;
}
#endif



