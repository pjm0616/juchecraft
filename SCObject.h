// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCObject_H_
#define SCObject_H_

namespace SC {

namespace ObjectType
{
	enum
	{
		None = 0, 
		Resource = 1, 
		Unit = 2, 
		Building = 3, 
	};
}
typedef unsigned int ObjectType_t;

// *Current* object state
namespace ObjectState
{
	enum
	{
		None = 0, 
		
		Burrowed = 1, 
		InTransit = 2, 
		Cloaked = 4, 
		Invincible = 8, 
		Hallucinated = 16, 
		
		Moving = 64, 
		Attacking = 128, // does not include pending attack
	};
}
typedef unsigned int ObjectState_t;

namespace ObjectId
{
#include "SCObjectIdList.h"
}
typedef unsigned int ObjectId_t;


class Game;
class Object
{
public:
	// MUST call init() immediately after contruction
	Object(Game *game);
	// MUST call cleanup() before destruction
	virtual ~Object();
	
	void init();
	void cleanup();
	
	void processFrame();
	
	/* Unit owner */
	Player *getOwner() { return this->owner; }
	void changeOwner(Player *new_owner);
	
	/* Unit position */
	float getX() const { return this->pos.getX(); }
	float getY() const { return this->pos.getY(); }
	void getPosition(float *x, float *y) const { this->pos.get(x, y); }
	void getPosition(int *x, int *y) const { this->pos.get(x, y); }
	const Coordinate &getPosition() const { return this->pos; }
	void setX(float x) { this->pos.setX(x); }
	void setY(float y) { this->pos.setY(y); }
	void setPosition(const Coordinate &pos) { this->pos.set(pos); }
	void setPosition(float x, float y) { this->pos.set(x, y); }
	void addX(float x) { this->pos.addX(x); }
	void addY(float y) { this->pos.addY(y); }
	void addPosition(const Coordinate &pos) { this->pos.add(pos); }
	void addPosition(float x, float y) { this->pos.add(x, y); }
	
	float getAngle() const { return this->angle; }
	float calculateAngle(const Coordinate &dest) const { return this->getPosition().calculateAngle(dest); }
	float calculateAngle(const Object *dest) const { return this->calculateAngle(dest->getPosition()); }
	
	/* Movement */
	// 공격, 정찰 등의 명령이 실행되었다면 stopAttack(), stopPatrol() 등을 호출 한 후 이동 함수 호출
	bool move(const Coordinate &dest, bool do_attack = false);
	bool move_centerAligned(const Coordinate &dest, bool do_attack = false);
	bool cmd_move(const Coordinate &dest, bool do_attack = false);
	bool cmd_move_centerAligned(const Coordinate &dest, bool do_attack = false);
	
	/* Attack */
	bool attack(Object *target);
	bool cmd_attack(Object *target);
	
	/* Unit state */
	bool isBurrowed() const { return (this->state & ObjectState::Burrowed); }
	bool isInTransit() const { return (this->state & ObjectState::InTransit); }
	bool isCloaked() const { return (this->state & ObjectState::Cloaked); }
	bool isInvincible() const { return (this->state & ObjectState::Invincible); }
	bool isHallucinated() const { return (this->state & ObjectState::Hallucinated); }
	bool isMoving() const { return (this->state & ObjectState::Moving); }
	bool isStopped() const { return !this->isMoving(); }
	bool isAttacking() const { return (this->state & ObjectState::Attacking); }
	
	/* unit abilities */
	bool canMove() const { return (this->getNetMovingSpeed() != 0.0); }
	bool canAttack() const { return (this->getNetDamage() != 0.0); }
	
	/* Variable object attributes */
	void setHP(float hp) { this->hit_point = hp; }
	void increaseHP(float hp) { this->hit_point += hp; }
	void decreaseHP(float hp) { this->hit_point -= hp; }
	void setEnergy(float energy) { this->energy = energy; }
	void increaseEnergy(float energy) { this->energy += energy; }
	void decreaseEnergy(float energy) { this->energy -= energy; }
	void setResource(int res) { this->remaining_resource = res; }
	void increaseResource(int res) { this->remaining_resource += res; }
	void decreaseResource(int res) { this->remaining_resource -= res; }
	
	float getHP() const { return this->hit_point; }
	float getEnergy() const { return this->energy; }
	int getResource() const { return this->remaining_resource; }
	
	float getNetArmor() const { return (this->getArmor() * this->getArmorBonusM()) + this->getArmorBonusA(); }
	float getNetDamage() const;
	float getNetMovingSpeed() const;
	float getNetAttackSpeed() const { return (this->getAttackSpeed() * this->getAttackSpeedBonusM()) + this->getAttackSpeedBonusA(); }
	float getNetAttackRange() const { return this->getAttackRange(); }
	
	/* Upgradable unit attributes */
	float getObjectArmorBonusA() const { return this->added_armor_bonus; }
	float getObjectDamageBonusA() const { return this->added_damage_bonus; }
	float getObjectMovingSpeedBonusA() const { return this->added_moving_speed_bonus; }
	float getObjectAttackSpeedBonusA() const { return this->added_attack_speed_bonus; }
	
	void setObjectArmorBonusA(float v) { this->added_armor_bonus = v; }
	void setObjectDamageBonusA(float v) { this->added_damage_bonus = v; }
	void setObjectMovingSpeedBonusA(float v) { this->added_moving_speed_bonus = v; }
	void setObjectAttackSpeedBonusA(float v) { this->added_attack_speed_bonus = v; }
	
	float getObjectArmorBonusM() const { return this->mul_armor_bonus; }
	float getObjectDamageBonusM() const { return this->mul_damage_bonus; }
	float getObjectMovingSpeedBonusM() const { return this->mul_moving_speed_bonus; }
	float getObjectAttackSpeedBonusM() const { return this->mul_attack_speed_bonus; }
	
	void setObjectArmorBonusM(float v) { this->mul_armor_bonus = v; }
	void setObjectDamageBonusM(float v) { this->mul_damage_bonus = v; }
	void setObjectMovingSpeedBonusM(float v) { this->mul_moving_speed_bonus = v; }
	void setObjectAttackSpeedBonusM(float v) { this->mul_attack_speed_bonus = v; }
	
	float getArmorBonusA() const { return this->owner->getPlayerArmorBonusA() + this->getObjectArmorBonusA();}
	float getDamageBonusA() const { return this->owner->getPlayerDamageBonusA() + this->getObjectDamageBonusA();}
	float getMovingSpeedBonusA() const { return this->owner->getPlayerMovingSpeedBonusA() + this->getObjectMovingSpeedBonusA();}
	float getAttackSpeedBonusA() const { return this->owner->getPlayerAttackSpeedBonusA() + this->getObjectAttackSpeedBonusA();}
	
	float getArmorBonusM() const { return this->getObjectArmorBonusM();}
	float getDamageBonusM() const { return this->getObjectDamageBonusM();}
	float getMovingSpeedBonusM() const { return this->getObjectMovingSpeedBonusM();}
	float getAttackSpeedBonusM() const { return this->getObjectAttackSpeedBonusM();}
	
protected:
	void setState(ObjectState_t state) { this->state = state; } // FIXME: change name
	void setState(ObjectState_t state, bool onoff);
	
	/* Movement */
	void setFinalDestination(const Coordinate &pos) { this->final_destination = pos; }
	const Coordinate &getFinalDestination() const { return this->final_destination; }
	void setDestination(const Coordinate &pos);
	const Coordinate &getDestination() const { return this->destination; }
	
private:
	/* Owner/Player related */
	void setOwner(Player *new_owner) { this->owner = new_owner; }
	void attachToOwner();
	void detachFromOwner();
	
	/* Movement related */
	Coordinate getMovementDelta(float time);
	void setMovementStartPoint(const Coordinate &pos) { this->movement_start_point = pos; }
	const Coordinate &getMovementStartPoint() const { return this->movement_start_point; }
	bool doMovement(float time);
	void stopMoving();
	
	/* Object owner/state/position etc. */
	void setAngle(float angle) { this->angle = angle; }
	
	/* Attack related */
	void setAttackTarget(Object *target) { this->attack_target = target; }
	Object *getAttackTarget() const { return this->attack_target; }
	// this와 dest간의 거리가 min_distance 이하일 경우 return true
	// 그렇지 않을 경우 where_to_move에 이동해야할 위치를 저장한 후 return false
	bool checkMinDistanceOld(Object *target, float min_distance, Coordinate *where_to_move);
	bool checkMinDistance(Object *target, float min_distance, Coordinate *where_to_move);
	bool doAttack(float time);
	void stopAttacking();
	
	/* Object owner/state/position etc. */
	Game *game;
	Player *owner;
	ObjectType_t type;
	ObjectState_t state;
	Coordinate pos;
	float angle;
	
	/* Object attributes */
	float hit_point, energy;
	int remaining_resource;
	float added_armor_bonus, added_damage_bonus, added_moving_speed_bonus, added_attack_speed_bonus;
	float mul_armor_bonus, mul_damage_bonus, mul_moving_speed_bonus, mul_attack_speed_bonus;
	
	/* Movement related */
	Coordinate movement_start_point, destination, final_destination;
	bool automatically_attack;
	
	/* Attack related */
	Object *attack_target; // not null if attack target is set.
	
	/////////////////////////////////////////////////////////////////////////
public: /* Constant object attributes */
	ObjectType_t getObjectType() const { return this->object_type; }
	ObjectId_t getObjectId() const { return this->object_id; }
	const char *getObjectIdName() const { return this->object_id_name; }
	const char *getObjectName() const { return this->object_name; }
	RaceId_t getRaceId() const { return this->race_id; }
	
	int getWidth() const { return this->width; }
	int getHeight() const { return this->height; }
	void getSize(int *w, int *h) const { *w = this->getWidth(); *h = this->getHeight(); }
	
	ObjectState_t getInitialState() const { return this->initial_state; }
	int getMaxHP() const { return this->max_hp; }
	int getMaxEnergy() const { return this->max_energy; }
	int getInitialResource() const { return this->initial_resource; }
	int getSuppliedFood() const { return this->supplied_food; }
	int getSuppliesInUse() const { return this->supplies_in_use; }
	
	// DO NOT use these function unless you want the RAW unit attribute(without upgrades).
	// use getNet**** instead.
	float getArmor() const { return this->armor; }
	float getDamage() const { return this->damage; }
	float getMovingSpeed() const { return this->moving_speed; }
	float getAttackSpeed() const { return this->attack_speed; }
	float getAttackRange() const { return this->attack_range; }
	
protected: /* Constant object attributes */
	ObjectType_t object_type;
	ObjectId_t object_id;
	const char *object_id_name, *object_name;
	RaceId_t race_id;
	
	ObjectState_t initial_state;
	int width, height;
	int max_hp, max_energy;
	int initial_resource;
	int supplied_food, supplies_in_use;
	
	float armor, damage;
	float moving_speed, attack_speed;
	float attack_range;
};


} // end of namespace SC

#define END_OF_SCObject_H_
#endif

