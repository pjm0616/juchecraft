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
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCObjectFactory.h"
#include "SCUnitAction.h"
#include "SCUnitCommand.h"
#include "SCPlayer.h"
#include "SCGame.h"

using namespace SC;


// default object properties. these will be modified in prototypes.
Object::ConstantAttributes::ConstantAttributes()
	:object_type(ObjectType::None), 
	object_id(ObjectId::None), 
	object_id_name("Object"), 
	object_name("Object"), 
	race_id(RaceId::None), 
	initial_state(ObjectState::None), 
	width(0), 
	height(0), 
	max_hp(-1), 
	max_energy(-1), 
	initial_minerals(0), 
	initial_vespene_gas(0), 
	provided_supplies(0), 
	required_supplies(0), 
	armor(0.0), 
	damage(0.0), 
	moving_speed(0.0), 
	attack_speed(0.0), 
	attack_range(0.0)
{
}

void Object::ConstantAttributes::operator=(const ConstantAttributes &o)
{
	#define COPY_MEMBER_VARIABLE(name_) this->name_ = o.name_
	COPY_MEMBER_VARIABLE(object_type);
	COPY_MEMBER_VARIABLE(object_id);
	COPY_MEMBER_VARIABLE(object_id_name);
	COPY_MEMBER_VARIABLE(object_name);
	COPY_MEMBER_VARIABLE(race_id);
	
	COPY_MEMBER_VARIABLE(initial_state);
	COPY_MEMBER_VARIABLE(width);
	COPY_MEMBER_VARIABLE(height);
	COPY_MEMBER_VARIABLE(max_hp);
	COPY_MEMBER_VARIABLE(max_energy);
	COPY_MEMBER_VARIABLE(initial_minerals);
	COPY_MEMBER_VARIABLE(initial_vespene_gas);
	COPY_MEMBER_VARIABLE(provided_supplies);
	COPY_MEMBER_VARIABLE(required_supplies);
	
	COPY_MEMBER_VARIABLE(armor);
	COPY_MEMBER_VARIABLE(damage);
	COPY_MEMBER_VARIABLE(moving_speed);
	COPY_MEMBER_VARIABLE(attack_speed);
	COPY_MEMBER_VARIABLE(attack_range);
	#undef COPY_MEMBER_VARIABLE
}


Object::Object(Game *game)
	:m_game(game)
{
	this->setState(ObjectState::None);
	
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
	
	this->clearActions();
	
	this->m_cleanup_called = false;
}

Object::~Object()
{
}

void Object::init()
{
	this->setOwner(this->m_game->getPlayer(Player::NeutralPlayer));
	this->attachToOwner();
	
	this->setState(this->getInitialState());
	this->setHP(this->getMaxHP());
	this->setEnergy(this->getMaxEnergy());
	this->setMinerals(this->getInitialMinerals());
	this->setVespeneGas(this->getInitialVespeneGas());
}

void Object::cleanup()
{
	if(this->m_cleanup_called == false)
	{
		this->m_cleanup_called = true;
		this->detachFromOwner();
	}
}


void Object::attachToOwner()
{
	this->getOwner()->increaseCurrentSupplies(this->getRaceId(), this->getProvidedSupplies());
	this->getOwner()->increaseSuppliesInUse(this->getRaceId(), this->getRequiredSupplies());
}

void Object::detachFromOwner()
{
	this->getOwner()->decreaseCurrentSupplies(this->getRaceId(), this->getProvidedSupplies());
	this->getOwner()->decreaseSuppliesInUse(this->getRaceId(), this->getRequiredSupplies());
}

void Object::changeOwner(const PlayerSPtr_t &new_owner)
{
	this->detachFromOwner();
	this->setOwner(new_owner);
	this->attachToOwner();
}



void Object::setState(ObjectState_t state, bool onoff)
{
	if(onoff)
		this->m_state |= state;
	else
		this->m_state &= ~state;
}


ObjectSPtr_t Object::clone()
{
	Object *obj = new Object(this->m_game);
	obj->m_constattrs = this->m_constattrs;
	
	return ObjectSPtr_t(obj);
}

void Object::clearActions()
{
	this->m_actions.clear();
}

float Object::getArmorBonusA() const { return this->m_owner->getPlayerArmorBonusA() + this->getObjectArmorBonusA();}
float Object::getDamageBonusA() const { return this->m_owner->getPlayerDamageBonusA() + this->getObjectDamageBonusA();}
float Object::getMovingSpeedBonusA() const { return this->m_owner->getPlayerMovingSpeedBonusA() + this->getObjectMovingSpeedBonusA();}
float Object::getAttackSpeedBonusA() const { return this->m_owner->getPlayerAttackSpeedBonusA() + this->getObjectAttackSpeedBonusA();}


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


#if 0
bool Object::doAttack(float time)
{
	assert(this->isAttacking() == true);
	assert(this->getAttackTarget() != NULL);
	
	const ObjectSPtr_t &target = this->getAttackTarget();
	Coordinate where_to_move;
	if(this->checkMinDistance(target, this->getNetAttackRange(), &where_to_move))
	{
		this->setAngle(this->getPosition().calculateAngle(target->getPosition()));
		
		double elapsed_time = this->m_game->getCachedElapsedTime();
		float attack_speed = this->getNetAttackSpeed(); // num_of_attacks per second
		float last_attack = this->getLastAttackTime();
		
		int nattacks = (elapsed_time - last_attack) / (1.0 / attack_speed);
		if(nattacks > 0)
			this->setLastAttackTime(elapsed_time);
		
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
				this->m_game->removeObject(target);
				// FIXME
				//target->kill();
				this->stopAttacking();
				break;
			}
		}
		return true;
	} /* if(this->checkMinDistance(target, this->getNetAttackRange(), &where_to_move)) */
	else
	{
		#if 0
		this->stopAttacking();
		fprintf(stderr, "Attack stopped\n");
		#else
		this->setState(ObjectState::Attacking, false);
		//this->move(where_to_move);
		this->move(target);
		#endif
		
		return false;
	} /* else */
}

void Object::stopAttacking()
{
	this->clearAttackTarget();
	this->setState(ObjectState::Attacking, false);
	
	//this->setLastAttackTime(this->m_game->getCachedElapsedTime()); // not necessary
}

bool Object::attack(const ObjectSPtr_t &target)
{
	if(!this->canAttack() || target->isInvincible() || target.get() == this)
	{
		return false;
	}
	
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
			this->clearAttackTarget();
			return false;
		}
	} /* if(this->checkMinDistance(target, range, NULL) == false) */
	else
	{
		this->setState(ObjectState::Attacking, true);
		this->setLastAttackTime(this->m_game->getCachedElapsedTime());
		//this->setAngle(this->getPosition().calculateAngle(target->getPosition()));
	}
	
	return true;
}

bool Object::cmd_attack(const ObjectSPtr_t &target)
{
	this->stopMoving();
	return this->attack(target);
}

bool Object::cmd_move(const Coordinate &dest, MovementFlags_t flags)
{
	this->stopAttacking();
	return this->move(dest, flags);
}
bool Object::cmd_move(const ObjectSPtr_t &target, float minimum_distance, MovementFlags_t flags)
{
	this->stopAttacking();
	return this->move(target, minimum_distance, flags);
}



#endif


void Object::processFrame()
{
	float deltat = this->m_game->getDelta();
	
	for(UnitActionTable::iterator it = this->m_actions.begin(); 
		it != this->m_actions.end(); 
		it++)
	{
		bool res = it->second.process(deltat);
	}
	#if 0
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
				this->setLastAttackTime(this->m_game->getCachedElapsedTime());
			}
		}
	}
	if(this->isAttacking())
	{
		this->doAttack(deltat);
	}
	#endif
	
	// TODO
}



bool Object::insideRect(int left, int top, int right, int bottom)
{
	int obj_cx, obj_cy;
	int obj_width, obj_height;
	this->getPosition(&obj_cx, &obj_cy);
	this->getSize(&obj_width, &obj_height);
	int obj_left = obj_cx - obj_width/2, obj_top = obj_cy - obj_height/2;
	int obj_right = obj_cx + obj_width/2, obj_bottom = obj_cy + obj_height/2;
	
	if(	(left <= obj_left && obj_right <= right) || 
		(obj_left <= left && left <= obj_right) || 
		(obj_left <= right && right <= obj_right))
	{
		if(	(top <= obj_top && obj_bottom <= bottom) || 
			(obj_top <= top && top <= obj_bottom) || 
			(obj_top <= bottom && bottom <= obj_bottom))
		{
			return true;
		}
	}
	return false;
}

bool Object::insideRect(const Coordinate &top_left, const Coordinate &bottom_right)
{
	Coordinate top_left2(top_left), bottom_right2(bottom_right);
	Coordinate::normalizeTopLeftCoordinate(top_left2, bottom_right2);
	
	return this->insideRect(top_left2.getX(), top_left2.getY(), bottom_right2.getX(), bottom_right2.getY());
}





