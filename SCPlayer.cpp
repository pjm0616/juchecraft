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
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCObjectIdList.h"
#include "SCObjectPrototypes.h"
#include "SCPlayer.h"
#include "SCGame.h"

using namespace SC;





Player::Player()
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
}


bool Player::isSelectedObject(const ObjectSPtr_t &obj) const
{
	for(ObjectList::const_iterator it = this->m_selected_objs.begin(); 
		it != this->m_selected_objs.end(); ++it)
	{
		if(*it == obj)
			return true;
	}
	return false;
}












