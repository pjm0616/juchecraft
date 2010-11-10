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
#include <cassert>

#ifdef DEBUG
#include <stdio.h>
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




UnitAction::UnitAction(UnitActionId_t actid)
	:m_actid(actid)
{
}
UnitAction::UnitAction(const ObjectSPtr_t &obj, UnitActionId_t actid)
	:m_actid(actid), 
	m_obj(obj)
{
	this->setObject(obj);
}
UnitAction::~UnitAction()
{
}

void UnitAction::setObject(const ObjectSPtr_t &obj)
{
	// if this->m_obj is already been set, throw an exception
	//assert(!this->m_obj);
	
	this->m_obj = obj;
}





UnitAction_Move::UnitAction_Move(const ObjectSPtr_t &obj, const Coordinate &dest, MovementFlags_t flags)
	:UnitAction(obj, UnitActionId::Move)
{
	this->move(dest, flags);
}
UnitAction_Move::UnitAction_Move(const ObjectSPtr_t &obj, const ObjectSPtr_t &target, float minimum_distance, MovementFlags_t flags)
	:UnitAction(obj, UnitActionId::Move)
{
	this->move(target, minimum_distance, flags);
}




bool UnitAction_Move::move(const Coordinate &dest, MovementFlags_t flags)
{
	if(!this->m_obj->canMove())
	{
		return false;
	}
	
	#if 0
	Coordinate next_pos = this->calculateNextDestination();
	#else
	Coordinate next_pos = dest;
	#endif
	
	this->setMovementFlags(flags);
	this->clearMovementTarget();
	this->setFinalDestination(dest);
	this->setDestination(next_pos);
	
	this->m_obj->setAngle(this->getMovementStartPoint().calculateAngle(this->getDestination()));
	
	return true;
}

bool UnitAction_Move::move(const ObjectSPtr_t &target, float minimum_distance, MovementFlags_t flags)
{
	if(!this->m_obj->canMove())
	{
		return false;
	}
	
	this->setMovementFlags(flags);
	this->setMovementTarget(target, minimum_distance);
	this->setFinalDestination(Coordinate(-1.0, -1.0)); // not necessary; our final destination is `target'
	this->setDestination(this->calculateDestination_TargetedMoving());
	
	this->m_obj->setAngle(this->getMovementStartPoint().calculateAngle(this->getDestination()));
	
	return true;
}


bool UnitAction_Move::process(float time)
{
	const ObjectSPtr_t &mvtarget = this->getMovementTarget();
	if(mvtarget)
	{
	#if 0
		this->getDestination().set(this->calculateDestination_TargetedMoving());
	#else
		float min_dist = this->getMovement_MinimumDistanceToTarget();
		if(this->checkMinDistance(mvtarget, min_dist, NULL))
		{
			return true;
		}
		else
		{
			this->getDestination().set(this->calculateDestination_TargetedMoving());
		}
	#endif
		this->m_obj->setAngle(this->getMovementStartPoint().calculateAngle(this->getDestination()));
	}
	
	float startx = this->getMovementStartPoint().getX();
	float starty = this->getMovementStartPoint().getY();
	float destx = this->getDestination().getX();
	float desty = this->getDestination().getY();
	Coordinate d_move = this->calculateMovementSpeed(time);
	Coordinate newpos = d_move + this->m_obj->getPosition();
	float newx = newpos.getX();
	float newy = newpos.getY();
	
	int nfinished = 0;
	if((startx >= destx && newx <= destx) // moving left
		|| (startx <= destx && newx >= destx)) // moving right
	{
		//fprintf(stderr, "X dir finished\n");
		this->m_obj->setX(destx);
		nfinished++;
	}
	else
	{
		this->m_obj->addX(d_move.getX());
	}
	
	if((starty >= desty && newy <= desty) // moving up
		|| (starty <= desty && newy >= desty)) // moving down
	{
		//fprintf(stderr, "Y dir finished\n");
		this->m_obj->setY(desty);
		nfinished++;
	}
	else
	{
		this->m_obj->addY(d_move.getY());
	}
	
	bool is_finished = (nfinished == 2);
	
	return is_finished;
}





void UnitAction_Move::setDestination(const Coordinate &pos)
{
	this->setMovementStartPoint(this->m_obj->getPosition());
	this->m_destination = pos;
}

Coordinate UnitAction_Move::calculateMovementSpeed(float time)
{
	float v = this->m_obj->getNetMovingSpeed();
	float startx = this->getMovementStartPoint().getX();
	float starty = this->getMovementStartPoint().getY();
	float destx = this->getDestination().getX();
	float desty = this->getDestination().getY();
	float x = this->m_obj->getX();
	float y = this->m_obj->getY();
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
		float distance = this->m_obj->getPosition().calculateDistance(this->getDestination());
		float t = distance / v;
		float vx = dx / t, vy = dy / t;
		
		//fprintf(stderr, "D2: v: %f, dist: %f, t: %f, dx: %f, vx: %f, dy: %f, vy: %f\n", v, distance, t, dx, vx, dy, vy);
		return Coordinate(vx * time, vy * time);
	}
}



Coordinate UnitAction_Move::calculateDestination_TargetedMoving()
{
	const ObjectSPtr_t &target = this->getMovementTarget();
	float min_dist = this->getMovement_MinimumDistanceToTarget();
	
	// TODO: implement this
	
	return target->getPosition();
}


static float calculate_distance(float x1, float y1, float x2, float y2)
{
	float dx = fabs(x2 - x1);
	float dy = fabs(y2 - y1);
	float distance = sqrt(dx*dx + dy*dy);
	return distance;
}



bool UnitAction_Move::checkMinDistanceOld(const ObjectSPtr_t &target, float min_distance, Coordinate *where_to_move)
{
	float my_xarr[4], target_xarr[4]; // 0, 1 = left, right
	float my_yarr[4], target_yarr[4]; // 0, 1 = top, bottom
	my_xarr[0] = this->m_obj->getX(); my_xarr[1] = this->m_obj->getX() + this->m_obj->getWidth();
	my_yarr[0] = this->m_obj->getY(), my_yarr[1] = this->m_obj->getY() + this->m_obj->getHeight();
	target_xarr[0] = target->getX(); target_xarr[1] = target->getX() + target->getWidth();
	target_yarr[0] = target->getY(), target_yarr[1] = target->getY() + target->getHeight();
	
	float distances[16];
	int idx = 0;
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			distances[idx++] = calculate_distance(my_xarr[j], my_yarr[i], target_xarr[j], target_yarr[i]);
			distances[idx++] = calculate_distance(my_xarr[j], my_yarr[i], target_xarr[j], target_yarr[!i]);
			distances[idx++] = calculate_distance(my_xarr[j], my_yarr[i], target_xarr[!j], target_yarr[i]);
			distances[idx++] = calculate_distance(my_xarr[j], my_yarr[i], target_xarr[!j], target_yarr[!i]);
		}
	}
	/*
		// 0, 1, 2, 3 = tl, tr, bl, br
		
		tl, tl
		tl, tr
		tl, bl
		tl, br
		
		tr, tl
		tr, rl
		tr, tb
		tr, rb
		
		bl, tl
		bl, rl
		bl, tb
		bl, rb
		
		br, tl
		br, rl
		br, tb
		br, rb
	*/
	
	int min = 0;
	for(int i = 1; i < 16; i++)
	{
		if(distances[i] <= distances[min])
		{
			min = i;
		}
	}
	
	if(distances[min] <= min_distance)
	{
		return true;
	}
	else
	{
		float x, y;
		
		// 0, 1, 2, 3 = tl, tr, bl, br
		int me_pnt = min / 4;
		int target_pnt = min % 4;
		
		x = y = 0.0; // FIXME
		
		where_to_move->set(x, y);
		return false;
	}
}

bool UnitAction_Move::checkMinDistance(const ObjectSPtr_t &target, float min_distance, Coordinate *where_to_move)
{
	float cx = this->m_obj->getX(), w = this->m_obj->getWidth(), target_cx = target->getX(), target_w = target->getWidth();
	float cy = this->m_obj->getY(), h = this->m_obj->getHeight(), target_cy = target->getY(), target_h = target->getHeight();
	float x = cx - w/2, y = cy - h/2;
	float target_x = target_cy - target_w/2, target_y = target_cy - target_h/2;
	float x_center = x + w/2, target_x_center = target_x + target_w/2;
	float y_center = y + h/2, target_y_center = target_y + target_h/2;
	float dx_center = target_x_center - x_center;
	float dy_center = target_y_center - y_center;
	
	Coordinate me_pnt;
	Coordinate target_pnt;
	if(dx_center < 0) // left
	{
		if(dy_center < 0) // top
		{
			me_pnt.set(x, y); // top left
			target_pnt.set(target_x + target_w, target_y + target_h); // bottom right
		}
		else // bottom
		{
			me_pnt.set(x, y + h); // bottom left
			target_pnt.set(target_x + target_h, target_y); // top right
		}
	}
	else // right
	{
		if(dy_center < 0) // top
		{
			me_pnt.set(x + w, y); // top right
			target_pnt.set(target_x, target_y + target_h); // bottom left
		}
		else // bottom
		{
			me_pnt.set(x + w, y + h); // bottom right
			target_pnt.set(target_x, target_y); // top left
		}
	}
	
	float distance = me_pnt.calculateDistance(target_pnt);
	if(distance <= min_distance)
	{
		return true;
	}
	else
	{
		if(where_to_move)
		{
			where_to_move->set(target->getPosition()); // FIXME!!
		}
		return false;
	}
}


