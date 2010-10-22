// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

/** @file SCObject.h
 *  @brief Implements abstract game object
**/


#ifndef SCObject_H_
#define SCObject_H_

namespace SC {


/** @brief Defines object types
 */
namespace ObjectType
{
	typedef unsigned int ObjectType_t;
	
	enum ObjectType
	{
		None = 0, 
		Resource = 1, 
		Unit = 2, 
		Building = 3, 
	};
}
using ObjectType::ObjectType_t;


/** @brief Defines current object state
 */
namespace ObjectState
{
	typedef unsigned int ObjectState_t;
	
	enum ObjectState
	{
		None = 0, /**< In most cases, this is default */
		
		Burrowed = 1, /**< Unit is burrowed */
		InTransit = 2, /**< Building is in transit */
		Cloaked = 4, /**< Object is cloaked */
		Invincible = 8, /**< Object is invincible */
		Hallucinated = 16, /**< Object is hallucinated */
		
		Moving = 64, /**< Object is moving */
		Attacking = 128, /**< Object is attacking @detail does not include pending attack */
	};
}
using ObjectState::ObjectState_t;

/** @brief Defines the list of object IDs
 */
namespace ObjectId
{
	typedef unsigned int ObjectId_t;
	enum ObjectId
	{
#		include "SCObjectIdList.h"
	};
}
using ObjectId::ObjectId_t;


class Game;
class Object
{
public:
	/** @name Constructor/destructor */
	//@{
	/** @details You MUST call SC::Object::init() IMMEDIATELY after construction.
	 *
	 *  @param[in] game A pointer to SC::Game object.
	 */
	Object(Game *game);
	/** @details You MUST call SC::Object::cleanup() before destruction.
	 */
	virtual ~Object();
	
	/** @brief initializes the object after ctor.
	 *  @details init() performs some initialization that uses virtual functions or variables that initialized at derived classes.
	 */
	void init();
	/** @brief initializes the object after ctor.
	 *  @details cleanup() performs some initialization that uses virtual functions.
	 */
	void cleanup();
	//@}
	
	/** @brief processes current frame.
	 *  @details This function is called from game main loop, in SC::Game.
	 */
	void processFrame();
	
	/** @name Unit owner related */
	//@{
	/** @brief Get player that owns this object.
	 *
	 *  @return A pointer to SC::Player that owns this object
	 */
	Player *getOwner() { return this->m_owner; }
	/** @brief Changes owner of the object.
	 *
	 *  @param[in] new_owner A pointer to SC::Player
	 */
	void changeOwner(Player *new_owner);
	//@}
	
	/** @name Unit position */
	//@{
	float getX() const { return this->m_pos.getX(); } /**< Get object's X coordinate */
	float getY() const { return this->m_pos.getY(); } /**< Get object's Y coordinate */
	void getPosition(float *x, float *y) const { this->m_pos.get(x, y); } /**< Get object's coordinate */
	void getPosition(int *x, int *y) const { this->m_pos.get(x, y); } /**< Get object's coordinate */
	const Coordinate &getPosition() const { return this->m_pos; } /**< Get object's coordinate */
	//@}
	//@{
	void setX(float x) { this->m_pos.setX(x); } /**< Set object's X coordinate */
	void setY(float y) { this->m_pos.setY(y); } /**< Set object's Y coordinate */
	void setPosition(const Coordinate &pos) { this->m_pos.set(pos); } /**< Set object's coordinate */
	void setPosition(float x, float y) { this->m_pos.set(x, y); } /**< Set object's coordinate */
	//@}
	//@{
	void addX(float x) { this->m_pos.addX(x); } /**< Increase object's X coordinate */
	void addY(float y) { this->m_pos.addY(y); } /**< Increase object's Y coordinate */
	void addPosition(const Coordinate &pos) { this->m_pos.add(pos); } /**< Increase object's coordinate */
	void addPosition(float x, float y) { this->m_pos.add(x, y); } /**< Increase object's coordinate */
	//@}
	
	/** @brief Get object's current angle.
	 *
	 *  @return The angle in degrees
	 */
	float getAngle() const { return this->m_angle; }
	//@{
	/** @brief Calculates angle from object to destination coordinate.
	 *
	 *  @return The angle in degrees
	 */
	float calculateAngle(const Coordinate &dest) const { return this->getPosition().calculateAngle(dest); }
	/** @brief Calculates angle from object to destination object.
	 *  
	 *  @return The angle in degrees
	 */
	float calculateAngle(const Object *dest) const { return this->calculateAngle(dest->getPosition()); }
	//@}
	
	/** @name Movement methods */
	//@{
	
	/** @brief Defines movement options.
	 *  @detail This is an enum class. C++98 does not support enum classes, damn.
	 */
	struct MovementFlags
	{
		enum
		{
			None = 0, 
			
			AutomaticallyAttack = 1, 
		};
	};
	typedef unsigned int MovementFlags_t;
	
	/** @brief Moves to top-left coordiate.
	 *
	 *  @param[in] dest Coordinate to destination
	 *  @param[in] flags Attack options. see MovementFlags.
	 *  @return true if succeeded. false if there's an error.
	 *  @sa MovementFlags
	 */
	bool move_notAligned(const Coordinate &dest, MovementFlags_t flags = MovementFlags::None);
	/** @brief Moves to coordinate, coordinate is corrected to unit's center.
	 *
	 *  @param[in] dest Coordinate to destination
	 *  @param[in] flags Attack options. see MovementFlags.
	 *  @return true if succeeded. false if there's an error.
	 *  @sa MovementFlags
	 */
	bool move(const Coordinate &dest, MovementFlags_t flags = MovementFlags::None);
	/** @brief Moves to target object.
	 *
	 *  @param[in] target The destination object to move to
	 *  @param[in] minimum_distance Minimum distance to target
	 *  @param[in] flags Attack options. see MovementFlags.
	 *  @return true if succeeded. false if there's an error.
	 *  @sa MovementFlags
	 */
	bool move(Object *target, float minumum_distance = 0.0, MovementFlags_t flags = MovementFlags::None);
	/** @brief Stops any other commands, and moves to coordinate.
	 *
	 *  @param[in] dest Coordinate to destination
	 *  @param[in] flags Attack options. see MovementFlags.
	 *  @return true if succeeded. false if there's an error.
	 *  @sa MovementFlags
	 */
	bool cmd_move_notAligned(const Coordinate &dest, MovementFlags_t flags = MovementFlags::None);
	/** @brief Stops any other commands, and moves to coordinate. Coordinate is corrected to unit's center.
	 *
	 *  @param[in] dest Coordinate to destination
	 *  @param[in] flags Attack options. see MovementFlags.
	 *  @return true if succeeded. false if there's an error.
	 *  @sa MovementFlags
	 */
	bool cmd_move(const Coordinate &dest, MovementFlags_t flags = MovementFlags::None);
	/** @brief Stops any other commands, and moves to target object.
	 *
	 *  @param[in] target The destination object to move to
	 *  @param[in] minimum_distance Minimum distance to target
	 *  @param[in] flags Attack options. see MovementFlags.
	 *  @return true if succeeded. false if there's an error.
	 *  @sa MovementFlags
	 */
	bool cmd_move(Object *target, float minumum_distance = 0.0, MovementFlags_t flags = MovementFlags::None);
	
	/** @brief 이동중인 시간을 구함.
	 *  @return 이동중인 시간
	 */
	float getMovingSeconds() const { return this->m_moving_secs; } // 이동중인 시간
	//@}
	
	/** @name Attack methods */
	//@{
	/** @brief Attacks target object.
	 *  @details If the distance to target is out of range, object moves to target.
	 *
	 *  @param[in] target The object to attack
	 *  @return true if succeeded. false if there's an error.
	 */
	bool attack(Object *target);
	/** @brief Stops any other commands, and attacks target object.
	 *  @details If the distance to target is out of range, object moves to target.
	 *
	 *  @param[in] target The object to attack
	 *  @return true if succeeded. false if there's an error.
	 */
	bool cmd_attack(Object *target);
	
	Object *getAttackTarget() const { return this->m_attack_target; }
	float getAttackingSeconds() const { return this->m_attacking_secs; } /**< 공격중인 시간 */
	float getLastAttackTime() const { return this->m_last_attack_time; } /**< 공격중인 시간을 기준으로 마지막 공격 시각 */
	//@}
	
	/** @name Unit states */
	/*@{*/
	
	/** @brief Get current unit state.
	 *
	 *  @return true or false
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
	
	/** @name Unit abilities */
	//@{
	bool canMove() const { return (this->getNetMovingSpeed() != 0.0); }
	bool canAttack() const { return (this->getNetDamage() != 0.0); }
	//@}
	
	/** @name Variable object attributes */
	//@{
	float getHP() const { return this->m_hit_point; }
	void setHP(float hp) { this->m_hit_point = hp; }
	void increaseHP(float hp) { this->m_hit_point += hp; }
	void decreaseHP(float hp) { this->m_hit_point -= hp; }
	
	float getEnergy() const { return this->m_energy; }
	void setEnergy(float energy) { this->m_energy = energy; }
	void increaseEnergy(float energy) { this->m_energy += energy; }
	void decreaseEnergy(float energy) { this->m_energy -= energy; }
	
	int getMinerals() const { return this->m_remaining_minerals; }
	void setMinerals(int res) { this->m_remaining_minerals = res; }
	void increaseMinerals(int res) { this->m_remaining_minerals += res; }
	void decreaseMinerals(int res) { this->m_remaining_minerals -= res; }
	
	int getVespeneGas() const { return this->m_remaining_vespene_gas; }
	void setVespeneGas(int res) { this->m_remaining_vespene_gas = res; }
	void increaseVespeneGas(int res) { this->m_remaining_vespene_gas += res; }
	void decreaseVespeneGas(int res) { this->m_remaining_vespene_gas -= res; }
	
	/** @brief Calculates total armor of the object.
	 *  @sa getArmor(), getArmorBonusA(), getArmorBonusM()
	 */
	float getNetArmor() const { return (this->getArmor() * this->getArmorBonusM()) + this->getArmorBonusA(); }
	/** @brief Calculates total damage of the object.
	 */
	float getNetDamage() const;
	/** @brief Calculates total moving speed of the object.
	 */
	float getNetMovingSpeed() const;
	/** @brief Calculates total attack speed of the object.
	 */
	float getNetAttackSpeed() const { return (this->getAttackSpeed() * this->getAttackSpeedBonusM()) + this->getAttackSpeedBonusA(); }
	/** @brief Calculates total attack range of the object.
	 */
	float getNetAttackRange() const { return this->getAttackRange(); }
	//@}
	
	/** @name Upgradable unit attributes */
	//@{
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
	
	//@{
	/** @brief Calculate total 'added armor bonuses'.
	 *  @sa getNetArmor()
	 */
	float getArmorBonusA() const { return this->m_owner->getPlayerArmorBonusA() + this->getObjectArmorBonusA();}
	/** @brief Calculate total 'added damage bonuses'.
	 *  @sa getNetArmor()
	 */
	float getDamageBonusA() const { return this->m_owner->getPlayerDamageBonusA() + this->getObjectDamageBonusA();}
	/** @brief Calculate total 'added moving speed bonuses'.
	 *  @sa getNetArmor()
	 */
	float getMovingSpeedBonusA() const { return this->m_owner->getPlayerMovingSpeedBonusA() + this->getObjectMovingSpeedBonusA();}
	/** @brief Calculate total 'added attack speed bonuses'.
	 *  @sa getNetArmor()
	 */
	float getAttackSpeedBonusA() const { return this->m_owner->getPlayerAttackSpeedBonusA() + this->getObjectAttackSpeedBonusA();}
	
	/** @brief Calculate total 'multiplied armor bonuses'.
	 *  @sa getNetArmor()
	 */
	float getArmorBonusM() const { return this->getObjectArmorBonusM();}
	/** @brief Calculate total 'multiplied damage bonuses'.
	 *  @sa getNetArmor()
	 */
	float getDamageBonusM() const { return this->getObjectDamageBonusM();}
	/** @brief Calculate total 'multiplied moving speed bonuses'.
	 *  @sa getNetArmor()
	 */
	float getMovingSpeedBonusM() const { return this->getObjectMovingSpeedBonusM();}
	/** @brief Calculate total 'multiplied attack speed bonuses'.
	 *  @sa getNetArmor()
	 */
	float getAttackSpeedBonusM() const { return this->getObjectAttackSpeedBonusM();}
	//@}
	
protected:
	/** @name Current unit states */
	//@{{
	/** @brief Get unit states.
	 *  @return unit's states
	 *  @sa ObjectState
	 */
	ObjectState_t getState() const { return this->m_state; }
	/** @brief Set unit states.
	 *  @param[in] state new unit state
	 *  @sa ObjectState
	 */
	void setState(ObjectState_t state) { this->m_state = state; } // FIXME: change name
	/** @brief Toggle specific unit state.
	 *  @param[in] state type of unit state to change
	 *  @param[in] onoff true or false
	 *  @sa ObjectState
	 */
	void setState(ObjectState_t state, bool onoff);
	//@}}
	
	/** @name Movement related */
	//@{
	void setFinalDestination(const Coordinate &pos) { this->m_final_destination = pos; }
	Coordinate &getFinalDestination() { return this->m_final_destination; }
	const Coordinate &getFinalDestination() const { return this->m_final_destination; }
	void setDestination(const Coordinate &pos);
	Coordinate &getDestination() { return this->m_destination; }
	const Coordinate &getDestination() const { return this->m_destination; }
	//@}
	
private:
	// No point having an accessor for m_game.
	//Game *getGame() { return this->m_game; }
			
	/** @name Unit owner related */
	//@{
	void setOwner(Player *new_owner) { this->m_owner = new_owner; }
	/** @brief Attaches to current owner.
	 *  @detail updates owner's suppliy statistics, etc.
	 */
	void attachToOwner();
	/** @brief Detaches from current owner.
	 *  @detail updates owner's suppliy statistics, etc.
	 */
	void detachFromOwner();
	//@}
	
	/** @name Movement related */
	//@{
	/** @brief 이 오브젝트가 움직인 시간 설정.
	 *  @detail only called my setMovementTarget()
	 */
	void setMovingSeconds(float time) { this->m_moving_secs = time; }
	/** @brief 이 오브젝트가 움직인 시간 증가.
	 *  @detail only called my setMovementTarget()
	 */
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
	/** @brief Processes movement.
	 *  @detail Called by game main loop.
	 *  @param[in] time this->game->getFrameDelta()
	 */
	bool doMovement(float time);
	void stopMoving();
	//@}
	
	/** @name Attack related */
	//@{
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
	/** @brief Processes attack.
	 *  @detail Called by game main loop.
	 *  @param[in] time this->game->getFrameDelta()
	 */
	bool doAttack(float time);
	void stopAttacking();
	//@}
	
	/** @name Object owner/state/position etc. */
	//@{
	void setAngle(float angle) { this->m_angle = angle; }
	//@}
	
	/** @name Object owner/state/position etc. */
	//@{
	Game *m_game;
	Player *m_owner;
	ObjectType_t m_type;
	ObjectState_t m_state;
	Coordinate m_pos;
	float m_angle;
	//@}
	
	/** @name Object attributes */
	//@{
	float m_hit_point; /**< Current hit point */
	float m_energy; /**< Current energy */
	int m_remaining_minerals; /**< Remaining minerals */
	int m_remaining_vespene_gas; /**< Remaining vespene gas */
	float m_added_armor_bonus, m_added_damage_bonus, m_added_moving_speed_bonus, m_added_attack_speed_bonus;
	float m_mul_armor_bonus, m_mul_damage_bonus, m_mul_moving_speed_bonus, m_mul_attack_speed_bonus;
	//@}
	
	/** @name Movement related */
	//@{
	float m_moving_secs; /**< 이동 시작 후 지금까지 총 시간 */
	Coordinate m_movement_start_point, m_destination, m_final_destination;
	MovementFlags_t m_movement_flags;
	// if target is set, object moves to target. is target is not set, object moves to coordinate.
	Object *m_movement_target;
	// if movement_target is set and movement_min_distance_to_target is set, 
	//		object moves to movement_target.getPosition(), but keeps distance `min. movement_min_distance_to_target' to target.
	float m_movement_min_distance_to_target; // TODO: implement this
	//@}
	
	/** @name Attack related */
	//@{
	float m_attacking_secs; /**< 공격 시작 후 지금까지 총 시간 */
	float m_last_attack_time; /**< `공격 시작 후 지금까지 총 시간'을 기준으로 마지막 공격 시각 */
	// if(this->m_isMoving() && this->getAttackTarget()) then this object is moving to attack target
	Object *m_attack_target; // not null if attack target is set.
	//@}
	
	/////////////////////////////////////////////////////////////////////////
public:
	/** @name Constant object attributes */
	//@{
	/** @sa ObjectType
	 */
	ObjectType_t getObjectType() const { return this->m_object_type; }
	/** @sa ObjectId
	 */
	ObjectId_t getObjectId() const { return this->m_object_id; }
	const char *getObjectIdName() const { return this->m_object_id_name; }
	const char *getObjectName() const { return this->m_object_name; }
	/** @sa RaceId
	 */
	RaceId_t getRaceId() const { return this->m_race_id; }
	
	int getWidth() const { return this->m_width; }
	int getHeight() const { return this->m_height; }
	void getSize(int *w, int *h) const { *w = this->getWidth(); *h = this->getHeight(); }
	
	/** @brief Get object's initial state.
	 *  @sa ObjectState
	 */
	ObjectState_t getInitialState() const { return this->m_initial_state; }
	int getMaxHP() const { return this->m_max_hp; }
	int getMaxEnergy() const { return this->m_max_energy; }
	int getInitialMinerals() const { return this->m_initial_minerals; }
	int getInitialVespeneGas() const { return this->m_initial_vespene_gas; }
	int getSuppliedFood() const { return this->m_supplied_food; }
	int getSuppliesInUse() const { return this->m_supplies_in_use; }
	
	/** @brief Get bare armor of the object.
	 *  @detail DO NOT use this function unless you want the RAW unit attribute(without upgrades).
	 *  @detail use getNetArmor() instead.
	 */
	float getArmor() const { return this->m_armor; }
	/** @brief Get bare damage of the object.
	 *  @detail DO NOT use this function unless you want the RAW unit attribute(without upgrades).
	 *  @detail use getNetDamage() instead.
	 */
	float getDamage() const { return this->m_damage; }
	/** @brief Get bare moving speed of the object.
	 *  @detail DO NOT use this function unless you want the RAW unit attribute(without upgrades).
	 *  @detail use getNetMovingSpeed() instead.
	 */
	float getMovingSpeed() const { return this->m_moving_speed; }
	/** @brief Get bare attack speed of the object.
	 *  @detail DO NOT use this function unless you want the RAW unit attribute(without upgrades).
	 *  @detail use getNetAttackSpeed() instead.
	 */
	float getAttackSpeed() const { return this->m_attack_speed; }
	/** @brief Get bare attack range of the object.
	 *  @detail DO NOT use this function unless you want the RAW unit attribute(without upgrades).
	 *  @detail use getNetAttackRange() instead.
	 */
	float getAttackRange() const { return this->m_attack_range; }
	//@}
	
protected:
	/** @name Constant object attributes */
	//@{
	ObjectType_t m_object_type; /**< Object type @sa ObjectType */
	ObjectId_t m_object_id; /**< Object ID @sa ObjectId */
	const char *m_object_id_name; /**< Name of ObjectId in string */
	const char *m_object_name; /**< Object's name */
	RaceId_t m_race_id; /**< Race of the object @sa RaceId */
	
	ObjectState_t m_initial_state; /**< Object's initial state @sa ObjectState */
	int m_width; /**< Width of the object */
	int m_height; /**< Height of the object */
	int m_max_hp; /**< Object's max hit point */
	int m_max_energy; /**< Object's max energy */
	int m_initial_minerals; /**< The initial amount of minerals */
	int m_initial_vespene_gas; /**< The initial amount of vespene gas */
	int m_supplied_food; /**< The amount of food this object supplies */
	int m_supplies_in_use; /**< The amount of food this object uses */
	
	float m_armor; /**< Bare armor of the object. Bonuses can be applied. @sa getNetArmor() */
	float m_damage; /**< Bare damage of the object @sa getNetDamage() */
	float m_moving_speed; /**< Bare moving speed of the object @sa getNetMovingSpeed() */
	float m_attack_speed; /**< Bare attack speed of the object @sa getNetAttackSpeed() */
	float m_attack_range; /**< Bare attack range of the object @sa getNetAttackRange() */
	//@}
	
private:
	bool m_cleanup_called; /**< for debugging */
};


} // end of namespace SC

#define END_OF_SCObject_H_
#endif

