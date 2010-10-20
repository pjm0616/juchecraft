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
	initial_state(ObjectState::None), 
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
	
	this->setMovementTarget(NULL);
	this->setAttackTarget(NULL);
}

Object::~Object()
{
}

void Object::init()
{
	this->setOwner(&Player::Players[Player::NeutralPlayer]);
	this->attachToOwner();
	
	this->setState(this->getInitialState());
	this->setHP(this->getMaxHP());
	this->setEnergy(this->getMaxEnergy());
	this->setResource(this->getInitialResource());
	
	this->cleanup_called = false; // for debugging
}

void Object::cleanup()
{
	assert(this->cleanup_called == false); this->cleanup_called = true; // for debugging
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
}

Coordinate Object::calculateMovementSpeed(float time)
{
	float v = this->getNetMovingSpeed();
	float startx = this->movement_start_point.getX();
	float starty = this->movement_start_point.getY();
	float destx = this->destination.getX();
	float desty = this->destination.getY();
	float x = this->getX();
	float y = this->getY();
	// 시작점 ~ 목적지
	float tdx = destx - startx;
	float tdy = desty - starty;
	// 현재위치 ~ 목적지
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
		float distance = this->getPosition().calculateDistance(this->getDestination());
		float t = distance / v;
		float vx = dx / t, vy = dy / t;
		
		//fprintf(stderr, "D2: v: %f, dist: %f, t: %f, dx: %f, vx: %f, dy: %f, vy: %f\n", v, distance, t, dx, vx, dy, vy);
		return Coordinate(vx * time, vy * time);
	}
}

bool Object::doMovement(float time)
{
	assert(this->isMoving() == true);
	
	Object *mvtarget = this->getMovementTarget();
	if(mvtarget)
	{
		this->destination.set(this->calculateDestination_TargetedMoving());
		this->setAngle(this->movement_start_point.calculateAngle(this->destination));
	}
	
	float startx = this->movement_start_point.getX();
	float starty = this->movement_start_point.getY();
	float destx = this->destination.getX();
	float desty = this->destination.getY();
	Coordinate d_move = this->calculateMovementSpeed(time);
	Coordinate newpos = d_move + this->getPosition();
	float newx = newpos.getX();
	float newy = newpos.getY();
	
	int nfinished = 0;
	if((startx >= destx && newx <= destx) // moving left
		|| (startx <= destx && newx >= destx)) // moving right
	{
		//fprintf(stderr, "X dir finished\n");
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
		//fprintf(stderr, "Y dir finished\n");
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



bool Object::move_notAligned(const Coordinate &dest, bool do_attack)
{
	if(!this->canMove())
		return false;
	
	#if 0
	Coordinate next_pos = this->calculateNextDestination();
	#else
	Coordinate next_pos = dest;
	#endif
	
	this->automatically_attack = do_attack;
	this->setMovementTarget(NULL);
	this->setFinalDestination(dest);
	this->setDestination(next_pos);
	this->setState(ObjectState::Moving, true);
	this->setMovingSeconds(0.0);
	this->setAngle(this->movement_start_point.calculateAngle(this->destination));
	
	return true;
}

bool Object::move(Object *target, float minimum_distance, bool do_attack)
{
	if(!this->canMove())
		return false;
	
	this->automatically_attack = do_attack;
	this->setMovementTarget(target, minimum_distance);
	this->setFinalDestination(Coordinate(-1.0, -1.0)); // not necessary; our final destination is `target'
	this->setDestination(this->calculateDestination_TargetedMoving());
	this->setState(ObjectState::Moving, true);
	this->setMovingSeconds(0.0);
	this->setAngle(this->movement_start_point.calculateAngle(this->destination));
	
	return true;
}

void Object::stopMoving()
{
	this->setState(ObjectState::Moving, false);
	this->setMovementTarget(NULL);
	
	this->setFinalDestination(Coordinate(0.0, 0.0)); // not necessary
	this->setDestination(Coordinate(0.0, 0.0)); // not necessary
	this->setMovementStartPoint(Coordinate(0.0, 0.0)); // not necessary
	this->setMovingSeconds(0.0); // not necessary
}

bool Object::move(const Coordinate &dest, bool do_attack)
{
	Coordinate dest2(dest);
	dest2.addX(-(this->getWidth() / 2));
	dest2.addY(-(this->getHeight() / 2));
	
	return this->move_notAligned(dest2, do_attack);
}


Coordinate Object::calculateDestination_TargetedMoving()
{
	Object *target = this->getMovementTarget();
	float min_dist = this->getMovement_MinimumDistanceToTarget();
	
	// TODO: implement this
	
	return target->getPosition();
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
bool Object::checkMinDistanceOld(Object *target, float min_distance, Coordinate *where_to_move)
{
	float my_xarr[4], de_xarr[4]; // 0, 1 = left, right
	float my_yarr[4], de_yarr[4]; // 0, 1 = top, bottom
	my_xarr[0] = this->getX(); my_xarr[1] = this->getX() + this->getWidth();
	my_yarr[0] = this->getY(), my_yarr[1] = this->getY() + this->getHeight();
	de_xarr[0] = target->getX(); de_xarr[1] = target->getX() + target->getWidth();
	de_yarr[0] = target->getY(), de_yarr[1] = target->getY() + target->getHeight();
	
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
		int target_pnt = min % 4;
		
		x = y = 0.0; // FIXME
		
		where_to_move->set(x, y);
		return false;
	}
}

bool Object::checkMinDistance(Object *target, float min_distance, Coordinate *where_to_move)
{
	float x = this->getX(), w = this->getWidth(), target_x = target->getX(), target_w = target->getWidth();
	float y = this->getY(), h = this->getHeight(), target_y = target->getY(), target_h = target->getHeight();
	float xc = x + w/2, target_xc = target_x + target_w/2;
	float yc = y + h/2, target_yc = target_y + target_h/2;
	float dxc = target_xc - xc;
	float dyc = target_yc - yc;
	
	Coordinate me_pnt;
	Coordinate target_pnt;
	if(dxc < 0) // left
	{
		if(dyc < 0) // top
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
		if(dyc < 0) // top
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


bool Object::doAttack(float time)
{
	assert(this->isAttacking() == true);
	assert(this->getAttackTarget() != NULL);
	
	Object *target = this->getAttackTarget();
	Coordinate where_to_move;
	if(this->checkMinDistance(target, this->getNetAttackRange(), &where_to_move))
	{
		this->setAngle(this->getPosition().calculateAngle(target->getPosition()));
		float attack_speed = this->getNetAttackSpeed(); // num_of_attacks per second
		float attacking_secs = this->getAttackingSeconds();
		float last_attack = this->getLastAttackTime();
		int nattacks = (attacking_secs - last_attack_time) / (1.0 / attack_speed);
		if(nattacks > 0)
			this->setLastAttackTime(attacking_secs);
		for(; nattacks > 0; nattacks--)
		{
			//fprintf(stderr, "Attack!\n");
			float t_damage = this->getNetDamage();
			float d_armor = target->getNetArmor();
			float net_damage = t_damage - d_armor;
		
			{ // debug
				float prev_hp = target->getHP();
				//fprintf(stderr, "Attack: damage: %f, armor: %f, netdamage: %f, hp: %f -> %f\n", 
				//	t_damage, d_armor, net_damage, prev_hp, prev_hp - net_damage);
			}
		
			target->decreaseHP(net_damage);
			float hp = target->getHP();
			if(hp <= 0)
			{
				this->stopAttacking();
				// FIXME
				//target->kill();
				this->game->removeObject(target);
				break;
			}
		}
		return true;
	}
	else
	{
		#if 1
		this->setState(ObjectState::Attacking, false);
		//this->move(where_to_move);
		this->move(target);
		#else
		this->stopAttacking();
		fprintf(stderr, "Attack stopped\n");
		#endif
		
		return false;
	}
}

void Object::stopAttacking()
{
	this->setAttackTarget(NULL);
	this->setState(ObjectState::Attacking, false);
	
	this->setAttackingSeconds(0.0); // not necessary
	this->setLastAttackTime(0.0); // not necessary
}

bool Object::attack(Object *target)
{
	if(!this->canAttack())
		return false;
	if(target->isInvincible())
		return false;
	float range = this->getNetAttackRange();
	
	this->setAttackTarget(target);
	if(this->checkMinDistance(target, range, NULL) == false)
	{
		if(this->canMove())
		{
			//this->move(where_to_move);
			this->move(target);
			this->setMovement_MinimumDistanceToTarget(range);
		}
		else
		{
			this->setAttackTarget(NULL);
			return false;
		}
	}
	else
	{
		this->setState(ObjectState::Attacking, true);
		this->setAttackingSeconds(0.0);
		this->setLastAttackTime(0.0);
		//this->setAngle(this->getPosition().calculateAngle(target->getPosition()));
	}
	
	return true;
}

bool Object::cmd_attack(Object *target)
{
	this->stopMoving();
	return this->attack(target);
}

bool Object::cmd_move_notAligned(const Coordinate &dest, bool do_attack)
{
	this->stopAttacking();
	return this->move_notAligned(dest, do_attack);
}
bool Object::cmd_move(const Coordinate &dest, bool do_attack)
{
	this->stopAttacking();
	return this->move(dest, do_attack);
}
bool Object::cmd_move(Object *target, float minimum_distance, bool do_attack)
{
	this->stopAttacking();
	return this->move(target, minimum_distance, do_attack);
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
				this->stopMoving();
			}
			else
			{
				//Coordinate next_pos = this->calculateNextDestination();
				//this->setDestination(next_pos);
			}
		#else
			
			this->stopMoving();
		#endif
			
			if(this->getAttackTarget() != NULL)
			{
				//this->attack(this->getAttackTarget());
				this->setState(ObjectState::Attacking, true);
				this->setAttackingSeconds(0.0);
				this->setLastAttackTime(0.0);
			}
		}
		this->increaseMovingSeconds(deltat);
	}
	if(this->isAttacking())
	{
		this->doAttack(deltat);
		this->increaseAttackingSeconds(deltat);
	}
	
	// TODO
}










