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
#include "game/Object.h"
#include "game/ObjectList.h"
#include "game/ObjectFactory.h"
#include "game/Player.h"
#include "game/Game.h"

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

ObjectPtr Object::clone()
{
	Object *obj = new Object(this->m_game);
	obj->m_constattrs = this->m_constattrs;
	
	return obj->makeThisPtr();
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

void Object::changeOwner(Player *new_owner)
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

float Object::getArmorBonusA() const { return this->getOwner()->getPlayerArmorBonusA() + this->getObjectArmorBonusA();}
float Object::getDamageBonusA() const { return this->getOwner()->getPlayerDamageBonusA() + this->getObjectDamageBonusA();}
float Object::getMovingSpeedBonusA() const { return this->getOwner()->getPlayerMovingSpeedBonusA() + this->getObjectMovingSpeedBonusA();}
float Object::getAttackSpeedBonusA() const { return this->getOwner()->getPlayerAttackSpeedBonusA() + this->getObjectAttackSpeedBonusA();}


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

void Object::processActions(float deltat)
{
	for(UnitAction::ActionTable::iterator it = this->m_actions.begin(); 
		it != this->m_actions.end(); )
	{
		const UnitAction::ActionPtr &act = it->second;
		if(likely(act))
		{
			bool res = act->process(deltat);
			if(res == true) // if finished then
			{
				this->m_actions.erase(it++);
			}
			else
			{
				++it;
			}
		}
		else
		{
			this->m_actions.erase(it++);
			SCAssert(!"Should not happen. as3o4ifs83u4c09m");
		}
	}
}

bool Object::processOrder(float deltat)
{
	if(this->m_order)
	{
		bool ret = this->m_order->process(deltat);
		if(!ret)
		{
			// order has finished
			this->clearActions();
			this->cancelOrder();
		}
		else
		{
			this->processActions(deltat);
		}
		
		return true;
	}
	else
	{
		return false;
	}
}

void Object::processFrame(float deltat)
{
	this->processOrder(deltat);
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


static float calculate_distance(float x1, float y1, float x2, float y2)
{
	float dx = fabs(x2 - x1);
	float dy = fabs(y2 - y1);
	float distance = sqrt(dx*dx + dy*dy);
	return distance;
}

bool Object::checkMinDistanceOld(const ObjectPtr &target, float min_distance, Coordinate *where_to_move)
{
	Object *obj = this;
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

bool Object::checkMinDistance(const ObjectPtr &target, float min_distance, Coordinate *where_to_move)
{
	Object *obj = this;
	float cx = obj->getX(), w = obj->getWidth(), target_cx = target->getX(), target_w = target->getWidth();
	float cy = obj->getY(), h = obj->getHeight(), target_cy = target->getY(), target_h = target->getHeight();
	float x = cx - w/2, target_x = target_cx - target_w/2;
	float y = cy - h/2, target_y = target_cy - target_h/2;
	float dx_center = target_cx - cx;
	float dy_center = target_cy - cy;
	
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




void Object::clearActions()
{
	this->m_actions.clear();
}

const UnitAction::ActionPtr &Object::getAction(UnitAction::ActionId_t action_id) const
{
	static UnitAction::ActionPtr null_obj;
	UnitAction::ActionTable::const_iterator it = this->m_actions.find(action_id);
	if(it == this->m_actions.end())
		return null_obj;
	else
		return it->second;
}

void Object::setAction(const UnitAction::ActionPtr &action)
{
	// TODO: check if action is a UnitAction::TargetedAction
	this->m_actions[action->getActionId()] = action;
}

bool Object::doAction(const UnitAction::ActionPtr &action)
{
	bool ret = action->initAction(this->getPtr());
	if(ret)
	{
		this->setAction(action);
	}
	return ret;
}



void Object::cancelOrder()
{
	this->m_order.reset();
}

bool Object::doOrder(const UnitOrder::OrderPtr &order)
{
	this->cancelOrder();
	this->clearActions();
	bool ret = order->initOrder(this->getPtr());
	if(ret)
	{
		this->m_order = order;
	}
	else
	{
		this->cancelOrder();
	}
	return ret;
}



bool Object::canAttack(const ObjectPtr &target) const
{
	if(this->canAttack()
		&& !target->isInvincible()
		&& likely(!target->isRemovedFromGame()) // should we do this?
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Object::hit(float damage)
{
	float d_armor = this->getNetArmor();
	float net_damage = damage - d_armor;
	
	this->decreaseHP(net_damage);
	return net_damage;
}


