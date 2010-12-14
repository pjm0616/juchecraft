/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *	
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *	
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/Object.h
 *  Implements abstract game object
**/

#ifndef SCObject_H_
#define SCObject_H_

namespace SC {

// TODO: move ObjectRenderState class to elsewhere
class ObjectRenderingState
{
public:
	ObjectRenderingState() {}
	virtual ~ObjectRenderingState() {}
};


/** 
 *  Abstract object class
 *  
 *  Warning: don't use ObjectPtr in Object. use ObjectWeakPtr instead.
 */
class Object: public WeakPtrOwner<Object>
{
	friend class ObjectFactory;
	friend class UnitAction::Action;
	
public:
	/** @name Constructor/destructor */
	//@{
	/** 
	 *  You MUST call Object::makeThisPtr() and store the pointer somewhere IMMEDIATELY after the construction.
	 *  You MUST call Object::init() before using this object.
	 *
	 *  @param[in] game A pointer to SC::Game object.
	 */
	Object(Game *game);
	/** 
	 *  You MUST call SC::Object::cleanup() before destruction.
	 */
	virtual ~Object();
	
	/** @brief initializes the object after ctor.
	 *  
	 *  init() performs some initialization that uses virtual functions or variables that initialized at derived classes.
	 *  this->m_game must be fully initialized before calling this function.
	 */
	void init();
	/** @brief cleans up the object before dtor.
	 *  
	 *  cleanup() performs some initialization that uses virtual functions.
	 */
	void cleanup();
	//@}
	
	/** checks if this object is removed from the game.
	 *  @return true if this object is removed from game.
	 */
	bool isRemovedFromGame() const { return this->m_cleanup_called; }
	
	/** processes current frame.
	 *  @details This function is called from game main loop, in SC::Game.
	 */
	void processFrame(float deltat);
	
	/** @name Unit owner related */
	//@{
	Player *getOwner() const { return this->m_owner; }
	void changeOwner(Player *new_owner);
	//@}
	
	Game *getGame() { return this->m_game; }
	
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
	
	void setSpeed(const Vector2 &speed) { this->m_speed = speed; }
	const Vector2 &getSpeed() const { return this->m_speed; }
	
	float getAngle() const { return this->m_angle; }
	void setAngle(float angle) { this->m_angle = angle; }
	
	//@{
	/** Calculates angle from object to destination coordinate.
	 *  @return The angle in degrees
	 */
	float calculateAngle(const Coordinate &dest) const { return this->getPosition().calculateAngle(dest); }
	/** Calculates angle from object to destination object.
	 *  @return The angle in degrees
	 */
	float calculateAngle(const ObjectPtr &dest) const { return this->calculateAngle(dest->getPosition()); }
	//@}
	
	/** Checks if this object is inside the rect
	 */
	bool insideRect(int left, int top, int right, int bottom);
	bool insideRect(const Coordinate &top_left, const Coordinate &bottom_right);
	
	/** @brief checks the distance between `this' and `dest'
	 * 
	 *  Checks if the distance if less(or equal) than `min_distance'.
	 *  if not, stores the coordinate to move in order to reach target in `where_to_move'.
	 *  @return true if the distance if less(or equal) than `min_distance'. Otherwise false.
	 */
	bool checkMinDistance(const ObjectPtr &target, float min_distance, Coordinate *where_to_move);
	
	/** checks if `this' and `obj' are currently collided
	 */
	bool testCollision(const ObjectPtr &obj);
	/** checks if `this' and `obj' will collide within next frame
	 */
	bool detectCollision(const ObjectPtr &obj);
	
	/** @name Unit states */
	//@{
	/** Get current unit state.
	 *  @return true or false
	 */
	bool isBurrowed() const { return (this->getState() & ObjectState::Burrowed); }
	bool isInTransit() const { return (this->getState() & ObjectState::InTransit); }
	bool isCloaked() const { return (this->getState() & ObjectState::Cloaked); }
	bool isInvincible() const { return (this->getState() & ObjectState::Invincible); }
	bool isHallucinated() const { return (this->getState() & ObjectState::Hallucinated); }
	//@{
	
	//@{
private:
	void clearAllActions();
	void setAction(const UnitAction::ActionPtr &action);
	
	void processActions(float deltat);
	void processOrder(float deltat);
public:
	//void cancelAction(UnitAction::ActionId_t actid); // TODO: implement this
	bool doAction(const UnitAction::ActionPtr &action);
	bool doAction(UnitAction::Action *action) { return this->doAction(UnitAction::ActionPtr(action)); }
	
	const UnitAction::ActionPtr &getAction(UnitAction::ActionId_t action_id) const;
	bool isActivatedAction(UnitAction::ActionId_t action_id) const { return (this->getAction(action_id) != NULL); }
	//@}
	
	//@{
	void cancelOrder();
	//void cancelSecondaryOrder();// there's no canceller for secondary order
	bool doOrder(const UnitOrder::OrderPtr &cmd);
	bool doOrder(UnitOrder::Order *cmd) { return this->doOrder(UnitOrder::OrderPtr(cmd)); }
	//@}
	
	//@{
	bool isMoving() const { return this->isActivatedAction(UnitAction::ActionId::Move); }
	bool isStopped() const { return !this->isMoving(); }
	bool isAttacking() const { return this->isActivatedAction(UnitAction::ActionId::Attack);; }
	//@}
	
	/** @name Unit abilities */
	//@{
	bool canMove() const { return (this->getNetMovingSpeed() != 0.0); }
	bool canAttack() const { return (this->getNetDamage() != 0.0); }
	//@}
	bool canAttack(const ObjectPtr &target) const;
	
	float hit(float damage);
	
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
	
	/** Calculates total armor of the object.
	 *  @sa getArmor(), getArmorBonusA(), getArmorBonusM()
	 */
	float getNetArmor() const { return (this->getArmor() * this->getArmorBonusM()) + this->getArmorBonusA(); }
	/** Calculates total damage of the object.
	 */
	float getNetDamage() const;
	/** Calculates total moving speed of the object.
	 */
	float getNetMovingSpeed() const;
	/** Calculates total attack speed of the object.
	 */
	float getNetAttackSpeed() const { return (this->getAttackSpeed() * this->getAttackSpeedBonusM()) + this->getAttackSpeedBonusA(); }
	/** Calculates total attack range of the object.
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
	/** Calculate total 'added armor bonuses'.
	 *  @sa getNetArmor()
	 */
	float getArmorBonusA() const;
	/** Calculate total 'added damage bonuses'.
	 *  @sa getNetArmor()
	 */
	float getDamageBonusA() const;
	/** Calculate total 'added moving speed bonuses'.
	 *  @sa getNetArmor()
	 */
	float getMovingSpeedBonusA() const;
	/** Calculate total 'added attack speed bonuses'.
	 *  @sa getNetArmor()
	 */
	float getAttackSpeedBonusA() const;
	
	/** Calculate total 'multiplied armor bonuses'.
	 *  @sa getNetArmor()
	 */
	float getArmorBonusM() const { return this->getObjectArmorBonusM();}
	/** Calculate total 'multiplied damage bonuses'.
	 *  @sa getNetArmor()
	 */
	float getDamageBonusM() const { return this->getObjectDamageBonusM();}
	/** Calculate total 'multiplied moving speed bonuses'.
	 *  @sa getNetArmor()
	 */
	float getMovingSpeedBonusM() const { return this->getObjectMovingSpeedBonusM();}
	/** Calculate total 'multiplied attack speed bonuses'.
	 *  @sa getNetArmor()
	 */
	float getAttackSpeedBonusM() const { return this->getObjectAttackSpeedBonusM();}
	//@}
	
public: /* protected */
	/** @name Current unit states */
	//@{{
	/** Get unit states.
	 *  @return unit's states
	 *  @sa ObjectState
	 */
	ObjectState_t getState() const { return this->m_state; }
	/** Set unit states.
	 *  @param[in] state new unit state
	 *  @sa ObjectState
	 */
	void setState(ObjectState_t state) { this->m_state = state; } // FIXME: change name
	/** Toggle specific unit state.
	 *  @param[in] state type of unit state to change
	 *  @param[in] onoff true or false
	 *  @sa ObjectState
	 */
	void setState(ObjectState_t state, bool onoff);
	//@}}
	
private:
	/** @name Unit owner related */
	//@{
	void setOwner(Player *new_owner) { this->m_owner = new_owner; }
	/** Attaches to current owner.
	 *  @details updates owner's supplys, etc.
	 */
	void attachToOwner();
	/** Detaches from current owner.
	 *  @details updates owner's supplys, etc.
	 */
	void detachFromOwner();
	//@}
	
public: // constant object attributes
	/** @sa ObjectType
	 */
	ObjectType_t getObjectType() const { return this->m_constattrs.object_type; }
	/** @sa ObjectId
	 */
	ObjectId_t getObjectId() const { return this->m_constattrs.object_id; }
	const char *getObjectIdName() const { return this->m_constattrs.object_id_name.c_str(); }
	const char *getObjectName() const { return this->m_constattrs.object_name.c_str(); }
	/** @sa RaceId
	 */
	RaceId_t getRaceId() const { return this->m_constattrs.race_id; }
	
	int getWidth() const { return this->m_constattrs.width; }
	int getHeight() const { return this->m_constattrs.height; }
	void getSize(int *w, int *h) const { *w = this->getWidth(); *h = this->getHeight(); }
	
	/** @name Get object's initial state.
	 *  @sa ObjectState
	 */
	//@{
	ObjectState_t getInitialState() const { return this->m_constattrs.initial_state; }
	int getMaxHP() const { return this->m_constattrs.max_hp; }
	int getMaxEnergy() const { return this->m_constattrs.max_energy; }
	int getInitialMinerals() const { return this->m_constattrs.initial_minerals; }
	int getInitialVespeneGas() const { return this->m_constattrs.initial_vespene_gas; }
	int getProvidedSupplies() const { return this->m_constattrs.provided_supplies; }
	int getRequiredSupplies() const { return this->m_constattrs.required_supplies; }
	//@}
	
	/** @name Unit's bare attributes (constant)
	 *  DO NOT use this function unless you want the RAW unit attribute(without upgrades).
	 *  use getNet*() instead.
	 */
	//@{
	float getArmor() const { return this->m_constattrs.armor; }
	float getDamage() const { return this->m_constattrs.damage; }
	float getMovingSpeed() const { return this->m_constattrs.moving_speed; }
	float getAttackSpeed() const { return this->m_constattrs.attack_speed; }
	float getAttackRange() const { return this->m_constattrs.attack_range; }
	//@}
	
	void setRenderingState(const ObjectRenderingStatePtr &newstate) { this->m_rendering_state = newstate; }
	void setRenderingState(ObjectRenderingState *newstate) { this->setRenderingState(ObjectRenderingStatePtr(newstate)); }
	const ObjectRenderingStatePtr &getRenderingStateRaw() { return this->m_rendering_state; }
	template <class T> inline T *getRenderingState()
	{
		T *rstate = dynastatic_cast<T *>(this->m_rendering_state.get());
		if(unlikely(!rstate))
		{
			rstate = new T;
			this->setRenderingState(rstate);
		}
		return rstate;
	}
	
public:
	/** Creates a new object based on this object.
	 *  @return Pointer to newly created object.
	 *  @sa SC::ObjectFactory
	 */
	ObjectPtr clone();
	

public:
	/** Manages unit production
	 *  @sa UnitProductionManager
	 */
	UnitProductionManager m_unit_producer;
private:
	bool m_cleanup_called;
	
	/** @name Object owner/state/position etc. */
	//@{
	Game *m_game; /**< Main game object */
	Player *m_owner; /**< The player that owns this object */
	ObjectState_t m_state; /**< See ObjectState */
	Coordinate m_pos;
	Vector2 m_speed;
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
	
	UnitAction::ActionTable m_actions; /**< List of currently activated actions */
	UnitOrder::OrderPtr m_order; /**< The order that this object is processing. */
	UnitOrder::OrderPtr m_secondary_order; /**< The order that this object is processing. */
	
	//std::list<UpgradeId> m_upgrade_queue; /**< Currently not used; todo: implement this */
	//std::deque<UnitOrderPtr> m_order_queue; /**< Currently not used; todo: implement this */
	
	/** Unit rendering state
	 */
	ObjectRenderingStatePtr m_rendering_state;
	//RenderingContext *m_renderctx; /**< TODO: contains information about rendering states. animation frame etc. It's not used in Object class */
	
	/** Constant object attributes */
	struct ConstantAttributes
	{
		ConstantAttributes();
		void operator=(const ConstantAttributes &o);
		
		ObjectType_t object_type; /**< Object type @sa ObjectType */
		ObjectId_t object_id; /**< Object ID @sa ObjectId */
		std::string object_id_name; /**< Name of ObjectId in string */
		std::string object_name; /**< Object's name */
		RaceId_t race_id; /**< Race of the object @sa RaceId */
	
		ObjectState_t initial_state; /**< Object's initial state @sa ObjectState */
		int width; /**< Width of the object */
		int height; /**< Height of the object */
		int max_hp; /**< Object's max hit point */
		int max_energy; /**< Object's max energy */
		int initial_minerals; /**< The initial amount of minerals */
		int initial_vespene_gas; /**< The initial amount of vespene gas */
		int provided_supplies; /**< The amount of supplies this object provide */
		int required_supplies; /**< The amount of supplies this object uses */
	
		float armor; /**< Bare armor of the object. Bonuses can be applied. @sa getNetArmor() */
		float damage; /**< Bare damage of the object @sa getNetDamage() */
		float moving_speed; /**< Bare moving speed of the object @sa getNetMovingSpeed() */
		float attack_speed; /**< Bare attack speed of the object @sa getNetAttackSpeed() */
		float attack_range; /**< Bare attack range of the object @sa getNetAttackRange() */
	} m_constattrs;
};


} // end of namespace SC

#define END_OF_SCObject_H_
#endif

