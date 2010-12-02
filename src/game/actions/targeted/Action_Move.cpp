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
#include "game/actions/Actions.h"
#include "game/orders/Orders.h"
#include "game/Object.h"
#include "game/ObjectList.h"
#include "game/ObjectFactory.h"
#include "game/Player.h"
#include "game/Game.h"

#include "game/actions/targeted/Action_Move.h"

using namespace SC;
using namespace SC::UnitAction;




Move::Move(const Target &target, float minimum_distance, MovementFlags_t flags)
	:TargetedAction(target, UnitAction::ActionId::Move)
{
	this->setMovementFlags(flags);
	this->setMinimumDistanceToTarget(minimum_distance);
	if(target.getType() == Target::Type::Coordinate)
		this->setFinalDestination(target.getCoordinate());
	// else: our final destination is `target'
	
	#if 0
	Coordinate next_pos = this->calculateNextDestination();
	#else
	Coordinate next_pos = target.getAsCoordinate();
	#endif
	this->setDestination(next_pos);
}

Move::~Move()
{
	this->cleanup();
}



bool Move::initAction(const ObjectPtr &obj)
{
	if(!this->TargetedAction::initAction(obj))
		return false;
	
	const Target &target = this->getTarget();
	if(	(obj->canMove() == false) || 
		(obj->getPosition() == this->getDestination())
		)
	{
		return false;
	}
	
	this->setStartPoint(obj->getPosition());
	
	// calculate angle if target is a coordinate
	if(target.isCoordinateTarget())
		obj->setAngle(this->getStartPoint().calculateAngle(this->getDestination()));
	
	this->setAsStarted(true);
	
	return true;
}

void Move::cleanup()
{
	
}


bool Move::process(float time)
{
	// if this function is called without being activated, return true (and remove this action in actionlist)
	SCAssert(this->isFinished() == false);
	ObjectPtr obj = this->getObject();
	const Target &target = this->getTarget();
	
	if(target.isObjectTarget())
	{
		const ObjectPtr &mvtarget = target.getObject();
	#if 0
		this->getDestination().set(this->calculateDestination_TargetedMoving());
	#else
		float min_dist = this->getMinimumDistanceToTarget();
		if(obj->checkMinDistance(mvtarget, min_dist, NULL))
		{
			return this->setAsFinished(true), true;
		}
		else
		{
			this->getDestination().set(this->calculateDestination_TargetedMoving());
		}
	#endif
		obj->setAngle(this->getStartPoint().calculateAngle(this->getDestination()));
	}
	
	float startx = this->getStartPoint().getX();
	float starty = this->getStartPoint().getY();
	float destx = this->getDestination().getX();
	float desty = this->getDestination().getY();
	Coordinate d_move = this->calculateSpeed(time);
	Coordinate newpos = d_move + obj->getPosition();
	float newx = newpos.getX();
	float newy = newpos.getY();
	
	int nfinished = 0;
	if((startx >= destx && newx <= destx) // moving left
		|| (startx <= destx && newx >= destx)) // moving right
	{
		//fprintf(stderr, "X dir finished\n");
		obj->setX(destx);
		nfinished++;
	}
	else
	{
		obj->addX(d_move.getX());
	}
	
	if((starty >= desty && newy <= desty) // moving up
		|| (starty <= desty && newy >= desty)) // moving down
	{
		//fprintf(stderr, "Y dir finished\n");
		obj->setY(desty);
		nfinished++;
	}
	else
	{
		obj->addY(d_move.getY());
	}
	
	bool is_finished = (nfinished == 2);
	if(is_finished)
		this->setAsFinished(true);
	
	return is_finished;
}





void Move::setDestination(const Coordinate &pos)
{
	this->m_destination = pos;
}

Coordinate Move::calculateSpeed(float time)
{
	ObjectPtr obj = this->getObject();
	float v = obj->getNetMovingSpeed();
	float startx = this->getStartPoint().getX();
	float starty = this->getStartPoint().getY();
	float destx = this->getDestination().getX();
	float desty = this->getDestination().getY();
	float x = obj->getX();
	float y = obj->getY();
	float tdx = destx - startx;
	float tdy = desty - starty;
	float dx = destx - x;
	float dy = desty - y;
	
	// compare signedness of total_dx and dx.
	bool is_xdir_finished = (tdx*dx <= 0);
	bool is_ydir_finished = (tdy*dy <= 0);
	if(is_xdir_finished && is_ydir_finished) // already at the destination
	{
		return Coordinate(0.0, 0.0);
	}
	else if(is_xdir_finished) // move vertically
	{
		int dy_sign = dy<0 ? -1 : 1;
		float v_y = v * dy_sign;
		//fprintf(stderr, "V: x: %f\tdx: %f\tvx: %f\tvx*t: %f\ty: %f\tdy: %f\tvy: %f\tvy*t: %f\n", x, dx, 0.0, 0.0, y, dy, v_y, v_y * time);
		return Coordinate(0.0, v_y * time);
	}
	else if(is_ydir_finished) // move horizontally
	{
		int dx_sign = dx<0 ? -1 : 1;
		float v_x = v * dx_sign;
		//fprintf(stderr, "H: x: %f\tdx: %f\tvx: %f\tvx*t: %f\ty: %f\tdy: %f\tvy: %f\tvy*t: %f\n", x, dx, v_x, v_x * time, y, dy, 0.0, 0.0);
		return Coordinate(v_x * time, 0.0);
	}
	else // move diagonally
	{
		float distance = obj->getPosition().calculateDistance(this->getDestination());
		float t = distance / v;
		float vx = dx / t, vy = dy / t;
		
		//fprintf(stderr, "D2: v: %f, dist: %f, t: %f, dx: %f, vx: %f, dy: %f, vy: %f\n", v, distance, t, dx, vx, dy, vy);
		return Coordinate(vx * time, vy * time);
	}
}



Coordinate Move::calculateDestination_TargetedMoving()
{
	const Target &target = this->getTarget();
	float min_dist = this->getMinimumDistanceToTarget();
	
	// TODO: implement this
	
	return target.getObject()->getPosition();
}


