/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/Target.cpp
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

using namespace SC;






Target::Target()
{
	this->clear();
}

Target::Target(const Coordinate &coord)
{
	this->set(coord);
}

Target::Target(const ObjectPtr &obj)
{
	this->set(obj);
}

Target::Target(const Target &o)
{
	switch(o.getType())
	{
	case Type::None:
		this->clear();
		break;
	case Type::Coordinate:
		this->set(o.getCoordinate());
		break;
	case Type::Object:
		this->set(o.getObject());
		break;
	default:
		SCAssert(!"Cannot happen. asiera-x384kp39c,9eu");
	}
}

Target::~Target()
{
	this->clear();
}



void Target::clear()
{
	this->m_type = Target::Type::None;
	this->m_target.obj.reset();
}
void Target::set(const Coordinate &coord)
{
	this->clear();
	this->m_type = Target::Type::Coordinate;
	this->m_target.coord = coord;
}
void Target::set(const ObjectPtr &obj)
{
	this->clear();
	this->m_type = Target::Type::Object;
	this->m_target.obj = obj;
}


const Coordinate &Target::getAsCoordinate() const
{
	SCAssert(this->getType() != Type::None);
	switch(this->getType())
	{
	case Type::Coordinate:
		return this->m_target.coord;
	case Type::Object:
		return this->m_target.obj->getPosition();
	default:
		SCAssert(!"Cannot happen. ao34h093");
		// Not reached
		return this->m_target.coord; // make compiler happy
	}
}



















