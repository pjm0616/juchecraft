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





UnitAction_Move::UnitAction_Move(const Coordinate &dest, MovementFlags_t flags)
	:UnitAction(UnitActionId::Move)
{
	this->move(dest, flags);
}
UnitAction_Move::UnitAction_Move(const ObjectPtr &target, float minimum_distance, MovementFlags_t flags)
	:UnitAction(UnitActionId::Move)
{
	this->move(target, minimum_distance, flags);
}




bool UnitAction_Move::move(const Coordinate &dest, MovementFlags_t flags)
{
	#if 0
	Coordinate next_pos = this->calculateNextDestination();
	#else
	Coordinate next_pos = dest;
	#endif
	
	this->setMovementFlags(flags);
	this->clearTarget();
	this->setFinalDestination(dest);
	this->setDestination(next_pos);
	
	return true;
}

bool UnitAction_Move::move(const ObjectPtr &target, float minimum_distance, MovementFlags_t flags)
{
	this->setMovementFlags(flags);
	this->setTarget(target, minimum_distance);
	this->setFinalDestination(Coordinate(-1.0, -1.0)); // not necessary; our final destination is `target'
	
	return true;
}


bool UnitAction_Move::process(const ObjectPtr &obj, float time)
{
	// if this function is called without being activated, return true (and remove this action in actionlist)
	if(unlikely(this->isFinished()))
		return true;
	const ObjectPtr &mvtarget = this->getTarget();
	
	// perform some initializations here
	// FIXME
	if(unlikely(this->isStarted() == false))
	{
		if(	!obj->canMove() || 
			(obj->getPosition() == this->getDestination())
			)
		{
			this->setAsFinished(true);
			return false;
		}
		this->setStartPoint(obj->getPosition());
		
		// calculate angle if target is a coordinate
		if(mvtarget == NULL)
			obj->setAngle(this->getStartPoint().calculateAngle(this->getDestination()));
		
		this->setAsStarted(true);
	}
	
	// start
	if(mvtarget)
	{
	#if 0
		this->getDestination().set(this->calculateDestination_TargetedMoving());
	#else
		float min_dist = this->getMinimumDistanceToTarget();
		if(this->checkMinDistance(obj, mvtarget, min_dist, NULL))
		{
			return true;
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
	Coordinate d_move = this->calculateSpeed(obj, time);
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





void UnitAction_Move::setDestination(const Coordinate &pos)
{
	this->m_destination = pos;
}

void UnitAction_Move::setTarget(const ObjectPtr &target, float minimum_distance)
{
	this->m_target = target;
	this->m_min_distance_to_target = minimum_distance;
}

Coordinate UnitAction_Move::calculateSpeed(const ObjectPtr &obj, float time)
{
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



Coordinate UnitAction_Move::calculateDestination_TargetedMoving()
{
	const ObjectPtr &target = this->getTarget();
	float min_dist = this->getMinimumDistanceToTarget();
	
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



bool UnitAction_Move::checkMinDistanceOld(const ObjectPtr &obj, const ObjectPtr &target, float min_distance, Coordinate *where_to_move)
{
	float my_xarr[4], target_xarr[4]; // 0, 1 = left, right
	float my_yarr[4], target_yarr[4]; // 0, 1 = top, bottom
	my_xarr[0] = obj->getX(); my_xarr[1] = obj->getX() + obj->getWidth();
	my_yarr[0] = obj->getY(), my_yarr[1] = obj->getY() + obj->getHeight();
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

bool UnitAction_Move::checkMinDistance(const ObjectPtr &obj, const ObjectPtr &target, float min_distance, Coordinate *where_to_move)
{
	float cx = obj->getX(), w = obj->getWidth(), target_cx = target->getX(), target_w = target->getWidth();
	float cy = obj->getY(), h = obj->getHeight(), target_cy = target->getY(), target_h = target->getHeight();
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


