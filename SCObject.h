// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCObject_H_
#define SCObject_H_

namespace SC {

namespace ObjectType
{
	enum ObjectType
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
	enum ObjectState
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

namespace MovementFlags
{
	enum MovementFlags
	{
		None = 0, 
		
		AutomaticallyAttack = 1, 
	};
}
typedef unsigned int MovementFlags_t;

namespace ObjectId
{
#include "SCObjectIdList.h"
}
typedef unsigned int ObjectId_t;


class Game;
class Object
{
public:
	/**
	 * @brief Constructor
	 * @details You MUST call SC::Object::init() IMMEDIATELY after construction
	 * @param[in] game A pointer to SC::Game object
	 */
	Object(Game *game);
	/**
	 * @brief Destructor
	 * @details You MUST call SC::Object::cleanup() before destruction
	 */
	virtual ~Object();
	
	/**
	 * @brief initializes the object after ctor
	 * @details init() performs some initialization that uses virtual functions or variables that initialized at derived classes
	 */
	void init();
	/**
	 * @brief initializes the object after ctor
	 * @details cleanup() performs some initialization that uses virtual functions
	 */
	void cleanup();
	
	/**
	 * @brief processes current frame
	 * @details This function is called from game main loop, in SC::Game
	 */
	void processFrame();
	
	// Unit owner
	/**
	 * @brief Get player that owns this object
	 * 
	 * @return A pointer to SC::Player that owns this object
	 */
	Player *getOwner() { return this->m_owner; }
	/**
	 * @brief Changes owner of the object
	 * 
	 * @param[in] new_owner A pointer to SC::Player
	 */
	void changeOwner(Player *new_owner);
	
	/* Unit position */
	//@{
	float getX() const { return this->m_pos.getX(); } /**< @brief Get object's X coordinate */
	float getY() const { return this->m_pos.getY(); } /**< @brief Get object's Y coordinate */
	void getPosition(float *x, float *y) const { this->m_pos.get(x, y); } /**< @brief Get object's coordinate */
	void getPosition(int *x, int *y) const { this->m_pos.get(x, y); } /**< @brief Get object's coordinate */
	const Coordinate &getPosition() const { return this->m_pos; } /**< @brief Get object's coordinate */
	//@}
	//@{
	void setX(float x) { this->m_pos.setX(x); } /**< @brief Set object's X coordinate */
	void setY(float y) { this->m_pos.setY(y); } /**< @brief Set object's Y coordinate */
	void setPosition(const Coordinate &pos) { this->m_pos.set(pos); } /**< @brief Set object's coordinate */
	void setPosition(float x, float y) { this->m_pos.set(x, y); } /**< @brief Set object's coordinate */
	//@}
	//@{
	void addX(float x) { this->m_pos.addX(x); } /**< @brief Increase object's X coordinate */
	void addY(float y) { this->m_pos.addY(y); } /**< @brief Increase object's Y coordinate */
	void addPosition(const Coordinate &pos) { this->m_pos.add(pos); } /**< @brief Increase object's coordinate */
	void addPosition(float x, float y) { this->m_pos.add(x, y); } /**< @brief Increase object's coordinate */
	//@}
	
	/**
	 * @brief Get object's current angle
	 * @return The angle in degrees
	 */
	float getAngle() const { return this->m_angle; }
	//@{
	/**
	 * @brief Calculates angle from object to destination
	 * @return The angle in degrees
	 */
	float calculateAngle(const Coordinate &dest) const { return this->getPosition().calculateAngle(dest); }
	/**
	 * @brief Calculates angle from object to destination
	 * @return The angle in degrees
	 */
	float calculateAngle(const Object *dest) const { return this->calculateAngle(dest->getPosition()); }
	//@}
	
	/* Movement */
	//@{
	/**
	 * @brief Moves to top-left coordiate.
	 *
	 * @param[in] dest Coordinate to destination
	 * @param[in] flags Attack options. see MovementFlags.
	 */
	bool move_notAligned(const Coordinate &dest, MovementFlags_t flags = MovementFlags::None);
	/**
	 * @brief Moves to coordinate, coordinate is corrected to unit's center.
	 *
	 * @param[in] dest Coordinate to destination
	 * @param[in] flags Attack options. see MovementFlags.
	 */
	bool move(const Coordinate &dest, MovementFlags_t flags = MovementFlags::None);
	/**
	 * @brief Moves to target object.
	 *
	 * @param[in] target The destination object to move to
	 * @param[in] minimum_distance Minimum distance to target
	 * @param[in] flags Attack options. see MovementFlags.
	 * @sa MovementFlags
	 */
	bool move(Object *target, float minumum_distance = 0.0, MovementFlags_t flags = MovementFlags::None);
	//@}
	//@{
	/**
	 * @brief Stops any other commands, and moves to coordinate.
	 *
	 * @param[in] dest Coordinate to destination
	 * @param[in] flags Attack options. see MovementFlags.
	 */
	bool cmd_move_notAligned(const Coordinate &dest, MovementFlags_t flags = MovementFlags::None);
	/**
	 * @brief Stops any other commands, and moves to coordinate. Coordinate is corrected to unit's center.
	 *
	 * @param[in] dest Coordinate to destination
	 * @param[in] flags Attack options. see MovementFlags.
	 */
	bool cmd_move(const Coordinate &dest, MovementFlags_t flags = MovementFlags::None);
	/**
	 * @brief Stops any other commands, and moves to target object.
	 *
	 * @param[in] target The destination object to move to
	 * @param[in] minimum_distance Minimum distance to target
	 * @param[in] flags Attack options. see MovementFlags.
	 */
	bool cmd_move(Object *target, float minumum_distance = 0.0, MovementFlags_t flags = MovementFlags::None);
	//@}
	
	/**
	 * @brief 이동중인 시간을 구함
	 * @return 이동중인 시간
	 */
	float getMovingSeconds() const { return this->m_moving_secs; } // 이동중인 시간
	
	/* Attack */
	/**
	 * @brief Attacks target object.
	 * @details If the distance to target is out of range, object moves to target.
	 * @param[in] target The object to attack
	 */
	bool attack(Object *target);
	/**
	 * @brief Stops any other commands, and attacks target object.
	 * @details If the distance to target is out of range, object moves to target.
	 * @param[in] target The object to attack
	 */
	bool cmd_attack(Object *target);
	
	Object *getAttackTarget() const { return this->m_attack_target; }
	float getAttackingSeconds() const { return this->m_attacking_secs; } // 공격중인 시간
	float getLastAttackTime() const { return this->m_last_attack_time; } // 공격중인 시간을 기준으로 마지막 공격 시각
	
	/* Unit state */
	
	/*@{*/
	/**
	 * @brief Get current unit state
	 * @return true or false
	 */
	bool isBurrowed() const { return (this->getState() & ObjectState::Burrowed); }
	bool isInTransit() const { return (this->getState() & ObjectState::InTransit); }
	bool isCloaked() const { return (this->getState() & ObjectState::Cloaked); }
	bool isInvincible() const { return (this->getState() & ObjectState::Invincible); }
	bool isHallucinated() const { return (this->getState() & ObjectState::Hallucinated); }
	bool isMoving() const { return (this->getState() & ObjectState::Moving); }
	bool isStopped() const { return !this->isMoving(); }
	bool isAttacking() const { return (this->getState() & ObjectState::Attacking); }
	/*@}*/
	
	/* unit abilities */
	bool canMove() const { return (this->getNetMovingSpeed() != 0.0); }
	bool canAttack() const { return (this->getNetDamage() != 0.0); }
	
	/* Variable object attributes */
	void setHP(float hp) { this->m_hit_point = hp; }
	void increaseHP(float hp) { this->m_hit_point += hp; }
	void decreaseHP(float hp) { this->m_hit_point -= hp; }
	void setEnergy(float energy) { this->m_energy = energy; }
	void increaseEnergy(float energy) { this->m_energy += energy; }
	void decreaseEnergy(float energy) { this->m_energy -= energy; }
	void setResource(int res) { this->m_remaining_resource = res; }
	void increaseResource(int res) { this->m_remaining_resource += res; }
	void decreaseResource(int res) { this->m_remaining_resource -= res; }
	
	float getHP() const { return this->m_hit_point; }
	float getEnergy() const { return this->m_energy; }
	int getResource() const { return this->m_remaining_resource; }
	
	float getNetArmor() const { return (this->getArmor() * this->getArmorBonusM()) + this->getArmorBonusA(); }
	float getNetDamage() const;
	float getNetMovingSpeed() const;
	float getNetAttackSpeed() const { return (this->getAttackSpeed() * this->getAttackSpeedBonusM()) + this->getAttackSpeedBonusA(); }
	float getNetAttackRange() const { return this->getAttackRange(); }
	
	/* Upgradable unit attributes */
	//@{
	/**
	 * @brief Get/set upgradable unit attributes
	 */
	float getObjectArmorBonusA() const { return this->m_added_armor_bonus; }
	float getObjectDamageBonusA() const { return this->m_added_damage_bonus; }
	float getObjectMovingSpeedBonusA() const { return this->m_added_moving_speed_bonus; }
	float getObjectAttackSpeedBonusA() const { return this->m_added_attack_speed_bonus; }
	
	void setObjectArmorBonusA(float v) { this->m_added_armor_bonus = v; }
	void setObjectDamageBonusA(float v) { this->m_added_damage_bonus = v; }
	void setObjectMovingSpeedBonusA(float v) { this->m_added_moving_speed_bonus = v; }
	void setObjectAttackSpeedBonusA(float v) { this->m_added_attack_speed_bonus = v; }
	
	float getObjectArmorBonusM() const { return this->m_mul_armor_bonus; }
	float getObjectDamageBonusM() const { return this->m_mul_damage_bonus; }
	float getObjectMovingSpeedBonusM() const { return this->m_mul_moving_speed_bonus; }
	float getObjectAttackSpeedBonusM() const { return this->m_mul_attack_speed_bonus; }
	
	void setObjectArmorBonusM(float v) { this->m_mul_armor_bonus = v; }
	void setObjectDamageBonusM(float v) { this->m_mul_damage_bonus = v; }
	void setObjectMovingSpeedBonusM(float v) { this->m_mul_moving_speed_bonus = v; }
	void setObjectAttackSpeedBonusM(float v) { this->m_mul_attack_speed_bonus = v; }
	//@}
	
	/**
	 * @brief Get total bonus value
	 */
	//@{
	float getArmorBonusA() const { return this->m_owner->getPlayerArmorBonusA() + this->getObjectArmorBonusA();}
	float getDamageBonusA() const { return this->m_owner->getPlayerDamageBonusA() + this->getObjectDamageBonusA();}
	float getMovingSpeedBonusA() const { return this->m_owner->getPlayerMovingSpeedBonusA() + this->getObjectMovingSpeedBonusA();}
	float getAttackSpeedBonusA() const { return this->m_owner->getPlayerAttackSpeedBonusA() + this->getObjectAttackSpeedBonusA();}
	
	float getArmorBonusM() const { return this->getObjectArmorBonusM();}
	float getDamageBonusM() const { return this->getObjectDamageBonusM();}
	float getMovingSpeedBonusM() const { return this->getObjectMovingSpeedBonusM();}
	float getAttackSpeedBonusM() const { return this->getObjectAttackSpeedBonusM();}
	//@}
	
protected:
	ObjectState_t getState() const { return this->m_state; }
	void setState(ObjectState_t state) { this->m_state = state; } // FIXME: change name
	void setState(ObjectState_t state, bool onoff);
	
	/* Movement */
	void setFinalDestination(const Coordinate &pos) { this->m_final_destination = pos; }
	Coordinate &getFinalDestination() { return this->m_final_destination; }
	const Coordinate &getFinalDestination() const { return this->m_final_destination; }
	void setDestination(const Coordinate &pos);
	Coordinate &getDestination() { return this->m_destination; }
	const Coordinate &getDestination() const { return this->m_destination; }
	
private:
	Game *getGame() { return this->m_game; }
			
	/* Owner/Player related */
	void setOwner(Player *new_owner) { this->m_owner = new_owner; }
	void attachToOwner();
	void detachFromOwner();
	
	/* Movement related */
	// only called my setMovementTarget
	void setMovingSeconds(float time) { this->m_moving_secs = time; }
	void increaseMovingSeconds(float time) { this->m_moving_secs += time; }
	void setMovement_MinimumDistanceToTarget(float distance) { this->m_movement_min_distance_to_target = distance; }
	float getMovement_MinimumDistanceToTarget() const { return this->m_movement_min_distance_to_target; }
	void setMovementTarget(Object *target, float minimum_distance = 0.0)
		{ this->m_movement_target = target; this->setMovement_MinimumDistanceToTarget(minimum_distance); }
	Object *getMovementTarget() const { return this->m_movement_target; }
	void setMovementStartPoint(const Coordinate &pos) { this->m_movement_start_point = pos; }
	const Coordinate &getMovementStartPoint() const { return this->m_movement_start_point; }
	
	Coordinate calculateDestination_TargetedMoving();
	Coordinate calculateMovementSpeed(float time);
	bool doMovement(float time);
	void stopMoving();
	
	/* Attack related */
	void setAttackTarget(Object *target) { this->m_attack_target = target; }
	void setAttackingSeconds(float time) { this->m_attacking_secs = time; }
	void increaseAttackingSeconds(float time) { this->m_attacking_secs += time; }
	void setLastAttackTime(float time) { this->m_last_attack_time = time; }
	void setMovementFlags(MovementFlags_t val) { this->m_movement_flags = val; }
	void setMovementFlags(MovementFlags_t type, bool onoff);
	MovementFlags_t getMovementFlags() const { return this->m_movement_flags; }
	bool getMovementFlag(MovementFlags_t type) const { return this->getMovementFlags() & type; }
	// this와 dest간의 거리가 min_distance 이하일 경우 return true
	// 그렇지 않을 경우 where_to_move에 이동해야할 위치를 저장한 후 return false
	bool checkMinDistanceOld(Object *target, float min_distance, Coordinate *where_to_move);
	bool checkMinDistance(Object *target, float min_distance, Coordinate *where_to_move);
	bool doAttack(float time);
	void stopAttacking();
	
	/* Object owner/state/position etc. */
	void setAngle(float angle) { this->m_angle = angle; }
	
	/* Object owner/state/position etc. */
	Game *m_game;
	Player *m_owner;
	ObjectType_t m_type;
	ObjectState_t m_state;
	Coordinate m_pos;
	float m_angle;
	
	/* Object attributes */
	float m_hit_point, m_energy;
	int m_remaining_resource;
	float m_added_armor_bonus, m_added_damage_bonus, m_added_moving_speed_bonus, m_added_attack_speed_bonus;
	float m_mul_armor_bonus, m_mul_damage_bonus, m_mul_moving_speed_bonus, m_mul_attack_speed_bonus;
	
	/* Movement related */
	float m_moving_secs; /**< 이동 시작 후 지금까지 총 시간 */
	Coordinate m_movement_start_point, m_destination, m_final_destination;
	MovementFlags_t m_movement_flags;
	// if target is set, object moves to target. is target is not set, object moves to coordinate.
	Object *m_movement_target;
	// if movement_target is set and movement_min_distance_to_target is set, 
	//		object moves to movement_target.getPosition(), but keeps distance `min. movement_min_distance_to_target' to target.
	float m_movement_min_distance_to_target; // TODO: implement this
	
	/* Attack related */
	float m_attacking_secs; /**< 공격 시작 후 지금까지 총 시간 */
	float m_last_attack_time; /**< `공격 시작 후 지금까지 총 시간'을 기준으로 마지막 공격 시각 */
	// if(this->m_isMoving() && this->getAttackTarget()) then this object is moving to attack target
	Object *m_attack_target; // not null if attack target is set.
	
	/////////////////////////////////////////////////////////////////////////
public: /* Constant object attributes */
	ObjectType_t getObjectType() const { return this->m_object_type; }
	ObjectId_t getObjectId() const { return this->m_object_id; }
	const char *getObjectIdName() const { return this->m_object_id_name; }
	const char *getObjectName() const { return this->m_object_name; }
	RaceId_t getRaceId() const { return this->m_race_id; }
	
	int getWidth() const { return this->m_width; }
	int getHeight() const { return this->m_height; }
	void getSize(int *w, int *h) const { *w = this->getWidth(); *h = this->getHeight(); }
	
	ObjectState_t getInitialState() const { return this->m_initial_state; }
	int getMaxHP() const { return this->m_max_hp; }
	int getMaxEnergy() const { return this->m_max_energy; }
	int getInitialResource() const { return this->m_initial_resource; }
	int getSuppliedFood() const { return this->m_supplied_food; }
	int getSuppliesInUse() const { return this->m_supplies_in_use; }
	
	// DO NOT use these function unless you want the RAW unit attribute(without upgrades).
	// use getNet**** instead.
	float getArmor() const { return this->m_armor; }
	float getDamage() const { return this->m_damage; }
	float getMovingSpeed() const { return this->m_moving_speed; }
	float getAttackSpeed() const { return this->m_attack_speed; }
	float getAttackRange() const { return this->m_attack_range; }
	
protected: /* Constant object attributes */
	ObjectType_t m_object_type;
	ObjectId_t m_object_id;
	const char *m_object_id_name, *m_object_name;
	RaceId_t m_race_id;
	
	ObjectState_t m_initial_state;
	int m_width, m_height;
	int m_max_hp, m_max_energy;
	int m_initial_resource;
	int m_supplied_food, m_supplies_in_use;
	
	float m_armor, m_damage;
	float m_moving_speed, m_attack_speed;
	float m_attack_range;
	
private:
	bool m_cleanup_called; // for debugging
};


} // end of namespace SC

#define END_OF_SCObject_H_
#endif

