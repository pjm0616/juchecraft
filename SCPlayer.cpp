// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"

#ifdef __WIN32__
#include <windows.h>
#endif

#include "smart_ptrs.h"
#include <string>
#include <list>
#include <map>
#include <vector>
#include <deque>
#include <algorithm>
#include <cassert>

#ifdef DEBUG
#include <cstdio>
#include <cstdlib>
#endif

#ifndef __WIN32__
#include <sys/time.h>
#endif

#include "defs.h"
#include "compat.h"
#include "luacpp/luacpp.h"
#include "SCTypes.h"
#include "SCException.h"
#include "SCCoordinate.h"
#include "SCObjectIdList.h"
#include "SCUnitAction.h"
#include "SCUnitCommand.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCObjectFactory.h"
#include "SCPlayer.h"
#include "SCGame.h"

using namespace SC;





Player::Player(Game *game)
	:m_game(game)
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
}


bool Player::isSelectedObject(const ObjectSPtr_t &obj) const
{
	ObjectList::const_iterator it = std::find(this->m_selected_objs.begin(), 
		this->m_selected_objs.end(), obj);
	if(it != this->m_selected_objs.end())
		return true;
	else
		return false;
}

void Player::filterCurSelectedObjects(ObjectList &selected_objs, int select_cnt_limit)
{
	int stats[ObjectType::SIZE] = {0, };
	int my_unit_cnt = 0;
	for(ObjectList::const_iterator it = selected_objs.begin(); 
		it != selected_objs.end(); ++it)
	{
		const ObjectSPtr_t &obj = *it;
		stats[obj->getObjectType()]++;
		if(obj->getOwner().get() == this)
			my_unit_cnt++;
	}

	for(ObjectList::iterator it = selected_objs.begin(); 
		it != selected_objs.end(); )
	{
		const ObjectSPtr_t &obj = *it;
		
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
			if(obj->getOwner().get() != this)
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
				this->m_selected_objs.addObject(*it);
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
	this->m_selection_in_progress = true;
	this->m_selection_start_coordinate = start_coord;
}

size_t Player::finishObjectSelection(const Coordinate &end_coord, SelectionFlags_t flags)
{
	ObjectList cur_selected_objs;
	int max_objs = 16;
	if(this->m_selection_start_coordinate == end_coord)
		max_objs = 1;
	
	this->m_game->findObjectByRect(cur_selected_objs, this->m_selection_start_coordinate, end_coord);
	this->filterCurSelectedObjects(cur_selected_objs, max_objs);
	this->mergeObjectList(this->m_selected_objs, cur_selected_objs, flags);
	
	this->m_selection_in_progress = false;
	return this->m_selected_objs.size();
}

size_t Player::getCurrentlySelectedObjects(ObjectList &buf, const Coordinate &crnt_coord, SelectionFlags_t flags)
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
	
	this->m_selection_in_progress = false;
	return buf.size();
}

size_t Player::selectObjects(const Coordinate &coord1, const Coordinate &coord2, SelectionFlags_t flags)
{
	this->startObjectSelection(coord1);
	return this->finishObjectSelection(coord2, flags);
}




const UnitCommand &Player::getFirstCommandInQueue() const
{
	static UnitCommand no_command(UnitCommandId::None);
	if(!this->m_cmdqueue.empty())
		return this->m_cmdqueue.front();
	else
	{
		return no_command;
	}
}








