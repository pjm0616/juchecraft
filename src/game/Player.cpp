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

/** @file src/game/Player.cpp
**/

#include "config.h"

#include "smart_ptrs.h"
#include <string>
#include <list>
#include <map>
#include <vector>
#include <deque>
#include <algorithm>

#ifdef DEBUG
#include <iostream>
#include <cstdio>
#include <cstdlib>
#endif

#ifndef __WIN32__
#include <sys/time.h>
#endif
#ifdef __WIN32__
#include <windows.h>
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
#include "ui/GameUI.h"

using namespace SC;





Player::Player(Game *game)
	: m_game(game)
{
	this->setPlayerId(-1);
	this->setPlayerColor(0x000000);
	this->setRace(RaceId::None);
	
	this->setMinerals(0);
	this->setVespeneGas(0);
	for(int i = 0; i < RaceId::Size; i++)
	{
		this->setCurrentSupplies(i, 0);
		this->setSuppliesInUse(i, 0);
	}
	
	this->setPlayerArmorBonusA(0.0);
	this->setPlayerDamageBonusA(0.0);
	this->setPlayerMovingSpeedBonusA(0.0);
	this->setPlayerAttackSpeedBonusA(0.0);
	
	this->m_selection_in_progress = false;
	
	this->clearOrder();
}

Player::~Player()
{
}

bool Player::isSelectedObject(const ObjectPtr &obj) const
{
	ObjectList::const_iterator it = std::find(this->m_selected_objs.begin(), 
		this->m_selected_objs.end(), obj);
	if(it != this->m_selected_objs.end())
		return true;
	else
		return false;
}

void Player::filterCurSelectedObjects(ObjectList &selected_objs, int select_cnt_limit) const
{
	int stats[ObjectType::SIZE] = {0, };
	int my_unit_cnt = 0;
	for(ObjectList::const_iterator it = selected_objs.begin(); 
		it != selected_objs.end(); ++it)
	{
		const ObjectPtr &obj = *it;
		stats[obj->getObjectType()]++;
		if(obj->getOwner() == this)
			my_unit_cnt++;
	}

	for(ObjectList::iterator it = selected_objs.begin(); 
		it != selected_objs.end(); )
	{
		const ObjectPtr &obj = *it;
		
		// if there are at least one selected unit, deselect non-unit objects.
		if(stats[ObjectType::Unit] > 0)
		{
			if(obj->getObjectType() != ObjectType::Unit)
				selected_objs.erase(it++);
			else
				++it;
		}
		else if(my_unit_cnt != 0)
		{
			if(obj->getOwner() != this)
				selected_objs.erase(it++);
			else
				++it;
		}
		else
		{
			++it;
		}
	} /* for(ObjectList::iterator it = selected_objs.begin(); */
	
	if(select_cnt_limit > 0)
	{
		ObjectList::iterator end = selected_objs.end();
		ObjectList::iterator it = selected_objs.begin();
		
		// skip first nth object
		for(int i = 0; i < select_cnt_limit && it != end; i++)
			++it;
		
		// erase everything else
		while(it != end)
			selected_objs.erase(it++);
	}
}

void Player::mergeObjectList(ObjectList &orig, const ObjectList &newobjs, SelectionFlags_t flags)
{
	if(flags == SelectionFlags::SET)
	{
		orig = newobjs;
	}
	else if(flags == SelectionFlags::ADD)
	{
		for(ObjectList::const_iterator it = newobjs.begin(); it != newobjs.end(); ++it)
		{
			ObjectList::const_iterator it2 = std::find(orig.begin(), orig.end(), *it);
			if(it2 == orig.end())
				orig.addObject(*it);
		}
	} /* else if(flags == SelectionFlags::ADD) */
	else if(flags == SelectionFlags::REMOVE)
	{
		for(ObjectList::const_iterator it = newobjs.begin(); it != newobjs.end(); ++it)
		{
			ObjectList::iterator it2 = std::find(orig.begin(), orig.end(), *it);
			if(it2 != orig.end())
				orig.erase(it2);
		}
	} /* else if(flags == SelectionFlags::REMOVE) */
}

void Player::startObjectSelection(const Coordinate &start_coord)
{
	this->clearOrder();
	
	this->m_selection_in_progress = true;
	this->m_selection_start_coordinate = start_coord;
}

size_t Player::finishObjectSelection(const Coordinate &end_coord, SelectionFlags_t flags)
{
	ObjectList cur_selected_objs;
	int max_objs = MAX_SELECTABLE_OBJS;
	if(this->m_selection_start_coordinate == end_coord)
		max_objs = 1;
	
	this->m_game->findObjectByRect(cur_selected_objs, this->m_selection_start_coordinate, end_coord);
	this->filterCurSelectedObjects(cur_selected_objs, max_objs);
	this->mergeObjectList(this->m_selected_objs, cur_selected_objs, flags);
	
	this->m_selection_in_progress = false;
	return this->m_selected_objs.size();
}

size_t Player::getCurrentlySelectedObjects(ObjectList &buf, const Coordinate &crnt_coord, SelectionFlags_t flags) const
{
	ObjectList cur_selected_objs;
	int max_objs = 16;
	if(this->m_selection_start_coordinate == crnt_coord)
		max_objs = 1;
	
	if(flags != SelectionFlags::SET)
		buf = this->m_selected_objs;
	
	this->m_game->findObjectByRect(cur_selected_objs, this->m_selection_start_coordinate, crnt_coord);
	this->filterCurSelectedObjects(cur_selected_objs, max_objs);
	this->mergeObjectList(buf, cur_selected_objs, flags);
	
	return buf.size();
}

size_t Player::selectObjects(const Coordinate &coord1, const Coordinate &coord2, SelectionFlags_t flags)
{
	this->startObjectSelection(coord1);
	return this->finishObjectSelection(coord2, flags);
}


void Player::setOrder(const UnitOrder::OrderPtr &order)
{
	this->m_order = order;
}
void Player::clearOrder()
{
	this->m_order.reset();
	//this->m_order.reset(new UnitOrder::NoOrder());
}

void Player::multiDoCurrentOrder()
{
	for(ObjectList::const_iterator it = this->m_selected_objs.begin(); 
		it != this->m_selected_objs.end(); ++it)
	{
		(*it)->doOrder(this->m_order->clone());
	}
	this->clearOrder();
}

void Player::multiCancelOrder()
{
	for(ObjectList::const_iterator it = this->m_selected_objs.begin(); 
		it != this->m_selected_objs.end(); ++it)
	{
		(*it)->cancelOrder();
	}
	this->clearOrder();
}

void Player::setOrderTargetByCoord(const Coordinate &coord)
{
	UnitOrder::TargetedOrder *tgorder = dynastatic_cast<UnitOrder::TargetedOrder *>(this->getOrder().get());
	SCAssert(tgorder != NULL);
	float x = coord.getX(), y = coord.getY();
	
	// check if there's a selectable object
	ObjectList dummy;
	ObjectPtr first = this->m_game->findObjectByRect(dummy, x, y, x, y);
	
	if(first)
		tgorder->setTarget(first);
	else
		tgorder->setTarget(coord);
}

void Player::setOrderTargetByRectCoord(Coordinate coord1, Coordinate coord2)
{
	UnitOrder::TargetedOrder *tgorder = dynastatic_cast<UnitOrder::TargetedOrder *>(this->getOrder().get());
	SCAssert(tgorder != NULL);
	Coordinate::normalizeTopLeftCoordinate(coord1, coord2);
	float x1 = coord1.getX(), y1 = coord1.getY();
	float x2 = coord2.getX(), y2 = coord2.getY();
	
	// check if there's a selectable object
	ObjectList dummy;
	ObjectPtr first = this->m_game->findObjectByRect(dummy, x1, y1, x2, y2);
	
	if(first)
	{
		tgorder->setTarget(first);
	}
	else
	{
		Coordinate center = (coord1 + coord2) / 2;
		tgorder->setTarget(center);
	}
}


#if 0
const UnitOrder::OrderPtr &Player::getFirstOrderInQueue() const
{
#if 0
	static UnitOrder::OrderPtr no_order;
#else
	static UnitOrder::OrderPtr no_order(new UnitOrder::NoOrder());
#endif
	if(!this->m_orderqueue.empty())
		return this->m_orderqueue.front();
	else
	{
		return no_order;
	}
}
#endif


void Player::toast(const std::string &msg, time_t duration)
{
	// FIXME: dirty hack
	if(duration == -1)
		duration = GameUI::Toast::DefaultDuration;
	
	this->m_game->getUI()->toast(msg, duration);
}






