// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifdef DEBUG
#include <stdio.h>
#include <cassert>
#endif

#include <tr1/memory>
#include <string>
#include <list>
#include <map>
#include <cmath>
#include <cstdlib>

#include "SCException.h"
#include "SCCoordinate.h"
#include "SCPlayer.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCGame.h"

using namespace SC;




Object::Object(Game *game)
	:game(game), 
	object_type(ObjectType::None), 
	object_id(ObjectId::None), 
	object_id_name("Object"), 
	object_name("Object"), 
	race_id(RaceId::None), 
	width(0), 
	height(0), 
	max_hp(-1), 
	max_energy(-1), 
	initial_resource(0), 
	supplied_food(0), 
	supplies_in_use(0), 
	armor(0.0), 
	damage(0.0), 
	moving_speed(0.0), 
	attack_speed(0.0), 
	attack_range(0.0)
{
	// meaningless
	this->setOwner(0);
	
	this->setHP(0);
	this->setEnergy(0);
	this->setResource(0);
	
	// meaningful
	this->state = ObjectState::None;
	
	this->setPosition(0.0, 0.0);
	
	this->setObjectArmorBonusA(0.0);
	this->setObjectDamageBonusA(0.0);
	this->setObjectMovingSpeedBonusA(0.0);
	this->setObjectAttackSpeedBonusA(0.0);
	
	this->setObjectArmorBonusM(1.0);
	this->setObjectDamageBonusM(1.0);
	this->setObjectMovingSpeedBonusM(1.0);
	this->setObjectAttackSpeedBonusM(1.0);
	
	this->setAngle(lrand48() % 360);
	
	this->setAttackTarget(NULL);
}

Object::~Object()
{
}

void Object::init()
{
	this->setOwner(&Player::Players[Player::NeutralPlayer]);
	this->attachToOwner();
	
	this->setHP(this->getMaxHP());
	this->setEnergy(this->getMaxEnergy());
	this->setResource(this->getInitialResource());
}

void Object::cleanup()
{
	this->detachFromOwner();
}


void Object::attachToOwner()
{
	this->getOwner()->increaseFoodMax(this->getRaceId(), this->getSuppliedFood());
	this->getOwner()->increaseFoodCrnt(this->getRaceId(), this->getSuppliesInUse());
}

void Object::detachFromOwner()
{
	this->getOwner()->decreaseFoodMax(this->getRaceId(), this->getSuppliedFood());
	this->getOwner()->decreaseFoodCrnt(this->getRaceId(), this->getSuppliesInUse());
}

void Object::changeOwner(Player *new_owner)
{
	this->detachFromOwner();
	this->setOwner(new_owner);
	this->attachToOwner();
}



void Object::setState(ObjectState_t state, bool onoff)
{
	if(onoff)
		this->state |= state;
	else
		this->state &= ~state;
}


float Object::getNetDamage() const
{
	float damage = this->getDamage();
	if(damage != 0.0)
	{
		damage *= this->getDamageBonusM();
		damage += this->getDamageBonusA();
	}
	return damage;
}

float Object::getNetMovingSpeed() const
{
	float mvspeed = this->getMovingSpeed();
	if(mvspeed != 0.0)
	{
		mvspeed *= this->getMovingSpeedBonusM();
		mvspeed += this->getMovingSpeedBonusA();
	}
	return mvspeed;
}


void Object::setDestination(const Coordinate &pos)
{
	this->setMovementStartPoint(this->getPosition());
	this->destination = pos;
	
	this->mov_y_err = 0.0;
	this->mov_x_err = 0.0;
}

Coordinate Object::getMovementDeltaOld(float time) const
{
	float v = this->getNetMovingSpeed();
	float dx = this->destination.getX() - this->getX();
	float dy = this->destination.getY() - this->getY();
	float abs_dx = fabs(dx), abs_dy = fabs(dy);
	int int_dx = dx, int_dy = dy;
	
	if(dx == 0.0 && dy == 0.0) // already at the destination
	{
		return Coordinate(0.0, 0.0);
	}
	else if(dx == 0.0) // move vertically
	{
		return Coordinate(0.0, v * time * (int_dy<0.0 ? -1 : 1));
	}
	else if(dy == 0.0) // move horizontally
	{
		return Coordinate(v * time * (int_dx<0.0 ? -1 : 1), 0.0);
	}
	else // move diagonally
	{
		// Vx^2 + Vy^2 = V^2 ; Vy = (dy*Vx) / dx
		// => Vx^2 + (dy*Vx/dx)^2 = V^2
		// => (Vx^2)dx + (Vx^2)(dy^2) = (V^2)dx
		// => (Vx^2)(dx + dy^2) = (V^2)dx
		// => Vx^2 = (V^2)dx / (dx + dy^2)
		// => Vx = sqrt((V^2)dx / (dx + dy^2))
		
		float v_x = std::sqrt( (v*v)*abs_dx / (abs_dx + (abs_dy*abs_dy)) );
		float v_y = abs_dy*v_x / abs_dx;
		if(dx < 0.0)
			v_x = -v_x;
		if(dy < 0.0)
			v_y = -v_y;
		
		return Coordinate(v_x * time, v_y * time);
	}
}
Coordinate Object::getMovementDelta(float time)
{
	float v = this->getNetMovingSpeed();
	float startx = this->movement_start_point.getX();
	float starty = this->movement_start_point.getY();
	float destx = this->destination.getX();
	float desty = this->destination.getY();
	float x = this->getX();
	float y = this->getY();
	// 시작점 ~ 목적지
	float tdx = destx - startx, abs_tdx = fabs(tdx);
	float tdy = desty - starty, abs_tdy = fabs(tdy);
	// 시작점 ~ 현재위치
	float sdx = x - startx, abs_sdx = fabs(sdx);
	float sdy = y - starty, abs_sdy = fabs(sdy);
	// 현재위치 ~ 목적지
	float dx = destx - x, abs_dx = fabs(dx);
	float dy = desty - y, abs_dy = fabs(dy);
	
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
		#if 0
			// Vx^2 + Vy^2 = V^2 ; Vy = (dy*Vx) / dx
			// => Vx^2 + (dy*Vx/dx)^2 = V^2
			// => (Vx^2)dx + (Vx^2)(dy^2) = (V^2)dx
			// => (Vx^2)(dx + dy^2) = (V^2)dx
			// => Vx^2 = (V^2)dx / (dx + dy^2)
			// => Vx = sqrt((V^2)dx / (dx + dy^2))
		
			#if 0
			float vx = std::sqrt( (v*v)*abs_dx / (abs_dx + (abs_dy*abs_dy)) );
			float vy = abs_dy*vx / abs_dx;
			#else
			float vx = std::sqrt( (v*v)*abs_tdx / (abs_tdx + (abs_tdy*abs_tdy)) );
			float vy = abs_tdy*vx / abs_tdx;
			#endif
			if(dx < 0.0)
				vx = -vx;
			if(dy < 0.0)
				vy = -vy;
		#else
			float distance = sqrt( abs_dx*abs_dx + abs_dy * abs_dy );
			float t = distance / v;
			float vx = dx / t, vy = dy / t;
		#endif
		
		//fprintf(stderr, "D2: v: %f, dist: %f, t: %f, dx: %f, vx: %f, dy: %f, vy: %f\n", v, distance, t, dx, vx, dy, vy);
		//fprintf(stderr, "D: x: %f\tdx: %f\tvx: %f\tvx*t: %f\ty: %f\tdy: %f\tvy: %f\tvy*t: %f\n", x, dx, vx, vx*time, y, dy, vy, vy*time);
		//fprintf(stderr, "D: v: %.02f  dx: %f vx: %f vx*t: %f\tdy: %f vy: %f vy*t: %f\tspeed: %.02f tspeed: %.02f\n", 
		//					   v, 		  dx, 	 vx, 	  vx*time,dy, 	 vy,	  vy*time,		 sqrt(vx*vx + vy*vy), v);
		return Coordinate(vx * time, vy * time);
		
		float integral_vx, fraction_vx;
		float integral_vy, fraction_vy;
		fraction_vx = modff(vx, &integral_vx);
		fraction_vy = modff(vy, &integral_vy);
		
		#if 1
			this->mov_y_err += vx;
			this->mov_x_err += vy;
			
			
		#else
		float xerr1 = this->mov_x_err, yerr1=this->mov_y_err;
		this->mov_x_err += fraction_vx;
		this->mov_y_err += fraction_vy;
		float xerr2 = this->mov_x_err, yerr2=this->mov_y_err;
		if(this->mov_x_err >= 0.5)
		{
			integral_vx++;
			this->mov_x_err -= 1.0;
		}
		if(this->mov_y_err >= 0.5)
		{
			integral_vy++;
			this->mov_y_err -= 1.0;
		}
		fprintf(stderr, "vx: %f  \t  xerr: %f -> %f -> %f  \t  vy: %f  \t  yerr: %f -> %f -> %f  \t  \n", vx, xerr1, xerr2, this->mov_x_err, vy, yerr1, yerr2, this->mov_y_err);
		
		return Coordinate(integral_vx * time, integral_vy * time);
		#endif
	}
}



bool Object::doMovement(float time)
{
	assert(this->isMoving() == true);
	float startx = this->movement_start_point.getX();
	float starty = this->movement_start_point.getY();
	float destx = this->destination.getX();
	float desty = this->destination.getY();
	Coordinate d_move = this->getMovementDelta(time);
	Coordinate newpos = d_move + this->getPosition();
	float newx = newpos.getX();
	float newy = newpos.getY();
	
	int nfinished = 0;
	if((startx >= destx && newx <= destx) // moving left
		|| (startx <= destx && newx >= destx)) // moving right
	{
		//fprintf(stderr, "X finished\n");
		this->setX(destx);
		nfinished++;
	}
	else
	{
		this->addX(d_move.getX());
	}
	
	if((starty >= desty && newy <= desty) // moving up
		|| (starty <= desty && newy >= desty)) // moving down
	{
		//fprintf(stderr, "Y finished\n");
		this->setY(desty);
		nfinished++;
	}
	else
	{
		this->addY(d_move.getY());
	}
	
	bool is_finished = (nfinished == 2);
	if(is_finished)
	{
		this->setState(ObjectState::Moving, false);
	}
	
	return is_finished;
}



void Object::move(const Coordinate &dest, bool do_attack)
{
	this->automatically_attack = do_attack;
	this->setFinalDestination(dest);
	#if 0
	Coordinate next_pos = this->calculateNextDestination();
	#else
	Coordinate next_pos = dest;
	#endif
	this->setDestination(next_pos);
	this->setState(ObjectState::Moving);
	
	float angle = this->movement_start_point.calculateAngle(this->destination);
	this->setAngle(angle);
}

void Object::move_centerAligned(const Coordinate &dest, bool do_attack)
{
	Coordinate dest2(dest);
	dest2.addX(-(this->getWidth() / 2));
	dest2.addY(-(this->getHeight() / 2));
	
	this->move(dest2, do_attack);
}

void Object::processFrame()
{
	float deltat = this->game->getFrameDelta();
	if(this->isMoving())
	{
		bool ret = doMovement(deltat);
		if(ret == true) // if finished then
		{
		#if 0
			if(this->getPosition() == this->getFinalDestination())
			{
				this->setState(ObjectState::Moving, false);
				
				this->setFinalDestination(Coordinate(0.0, 0.0)); // not necessary
				this->setDestination(Coordinate(0.0, 0.0)); // not necessary
				this->setMovementStartPoint(Coordinate(0.0, 0.0)); // not necessary
			}
			else
			{
				//Coordinate next_pos = this->calculateNextDestination();
				//this->setDestination(next_pos);
			}
		#else
			
			this->setFinalDestination(Coordinate(0.0, 0.0)); // not necessary
			this->setDestination(Coordinate(0.0, 0.0)); // not necessary
			this->setMovementStartPoint(Coordinate(0.0, 0.0)); // not necessary
		#endif
			
			if(this->getAttackTarget() != NULL)
			{
				//this->attack(this->getAttackTarget());
				this->setState(ObjectState::Attacking, true);
			}
		}
	}
	if(this->isAttacking())
	{
		this->doAttack(deltat);
	}
	
	// TODO
}

static float calculateDistance(float x1, float y1, float x2, float y2)
{
	float dx = fabs(x2 - x1);
	float dy = fabs(y2 - y1);
	float distance = sqrt(dx*dx + dy*dy);
	return distance;
}


// 이렇게 하지 말고, 아래 코드 다 날리고 -.-
// 나와 대상간의 각도를 구한 후
// 그걸로 최단거리가 되는 방향을 구한 후
// 최단거리는 한번만 계산함
bool Object::checkMinDistance(Object *dest, float min_distance, Coordinate *where_to_move)
{
	float my_xarr[4], de_xarr[4]; // 0, 1 = left, right
	float my_yarr[4], de_yarr[4]; // 0, 1 = top, bottom
	my_xarr[0] = this->getX(); my_xarr[1] = this->getX() + this->getWidth();
	my_yarr[0] = this->getY(), my_yarr[1] = this->getY() + this->getHeight();
	de_xarr[0] = dest->getX(); de_xarr[1] = dest->getX() + dest->getWidth();
	de_yarr[0] = dest->getY(), de_yarr[1] = dest->getY() + dest->getHeight();
	
	float distances[16];
	int idx = 0;
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			distances[idx++] = calculateDistance(my_xarr[j], my_yarr[i], de_xarr[j], de_yarr[i]);
			distances[idx++] = calculateDistance(my_xarr[j], my_yarr[i], de_xarr[j], de_yarr[!i]);
			distances[idx++] = calculateDistance(my_xarr[j], my_yarr[i], de_xarr[!j], de_yarr[i]);
			distances[idx++] = calculateDistance(my_xarr[j], my_yarr[i], de_xarr[!j], de_yarr[!i]);
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
			min = i;
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
		int dest_pnt = min % 4;
		
		x = y = 0.0; // FIXME
		
		where_to_move->set(x, y);
		return false;
	}
}


bool Object::doAttack(float time)
{
	assert(this->isAttacking() == true);
	assert(this->getAttackTarget() != NULL);
	
	Object *target = this->getAttackTarget();
	if(this->checkMinDistance(target, this->getNetAttackRange(), NULL))
	{
		float t_damage = this->getNetDamage();
		float d_armor = target->getNetArmor();
		
		float net_damage = t_damage - d_armor;
		target->decreaseHP(net_damage);
		
		return true;
	}
	else
	{
		#if 0
		this->setState(ObjectState::Attacking, false);
		this->move(where_to_move);
		#else
		this->stopAttack();
		#endif
		
		return false;
	}
}

void Object::stopAttack()
{
	this->setAttackTarget(NULL);
	this->setState(ObjectState::Attacking, false);
}

void Object::attack(Object *dest)
{
	Coordinate where_to_move;
	
	this->setAttackTarget(dest);
	if(this->checkMinDistance(dest, this->getNetAttackRange(), &where_to_move) == false)
	{
		this->move(where_to_move);
	}
	else
	{
		this->setState(ObjectState::Attacking, true);
	}
}


















