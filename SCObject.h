// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

/** @file SCObject.h
 *  @brief Implements abstract game object
**/


#ifndef SCObject_H_
#define SCObject_H_

#include "actions/UnitActionList.h" // FIXME: move this to elsewhere

namespace SC {



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
	void processFrame();
	
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
	
	float getAngle() const { return this->m_angle; }
	void setAngle(float angle) { this->m_angle = angle; }
	
	//@{
	/** @brief Calculates angle from object to destination coordinate.
	 *  @return The angle in degrees
	 */
	float calculateAngle(const Coordinate &dest) const { return this->getPosition().calculateAngle(dest); }
	/** @brief Calculates angle from object to destination object.
	 *  @return The angle in degrees
	 */
	float calculateAngle(const ObjectPtr &dest) const { return this->calculateAngle(dest->getPosition()); }
	//@}
	
	/** @brief Checks if this object is inside the rect
	 */
	bool insideRect(int left, int top, int right, int bottom);
	bool insideRect(const Coordinate &top_left, const Coordinate &bottom_right);
	
	
	//@{
	bool checkMinDistanceOld(const ObjectPtr &target, float min_distance, Coordinate *where_to_move);
	/** @brief checks the distance between `this' and `dest'
	 *  @detail Checks if the distance if less(or equal) than `min_distance'.
	 *  @detail if not, stores the coordinate to move in order to reach target in `where_to_move'.
	 *  @return true if the distance if less(or equal) than `min_distance'. Otherwise false.
	 */
	bool checkMinDistance(const ObjectPtr &target, float min_distance, Coordinate *where_to_move);
	//@}
	
	/** @name Unit states */
	//@{
	/** @brief Get current unit state.
	 *
	 *  @return true or false
	 */
	bool isBurrowed() const { return (this->getState() & ObjectState::Burrowed); }
	bool isInTransit() const { return (this->getState() & ObjectState::InTransit); }
	bool isCloaked() const { return (this->getState() & ObjectState::Cloaked); }
	bool isInvincible() const { return (this->getState() & ObjectState::Invincible); }
	bool isHallucinated() const { return (this->getState() & ObjectState::Hallucinated); }
	//@{
	
	//@{
	void clearActions();
private:
	void setAction(const UnitAction::ActionPtr &action);
public:
	bool doAction(const UnitAction::ActionPtr &action);
	bool doAction(UnitAction::Action *action) { return this->doAction(UnitAction::ActionPtr(action)); }
	
	const UnitAction::ActionPtr &getAction(UnitAction::ActionId_t action_id) const;
	bool isActivatedAction(UnitAction::ActionId_t action_id) const { return (this->getAction(action_id) != NULL); }
	//@}
	
	//@{
	bool move(const Coordinate &pos, UnitAction::Move::MovementFlags_t flags = UnitAction::Move::MovementFlags::None)
	{
		return this->doAction(new UnitAction::Move(pos, flags));
	}
	bool attack(const ObjectPtr &target)
	{
		return this->doAction(new UnitAction::Attack(target));
	}
	bool cmd_move(const Coordinate &pos, UnitAction::Move::MovementFlags_t flags = UnitAction::Move::MovementFlags::None)
	{ this->clearActions(); return this->move(pos, flags); }
	bool cmd_attack(const ObjectPtr &target)
	{ this->clearActions(); return this->attack(target); }
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
	float getArmorBonusA() const;
	/** @brief Calculate total 'added damage bonuses'.
	 *  @sa getNetArmor()
	 */
	float getDamageBonusA() const;
	/** @brief Calculate total 'added moving speed bonuses'.
	 *  @sa getNetArmor()
	 */
	float getMovingSpeedBonusA() const;
	/** @brief Calculate total 'added attack speed bonuses'.
	 *  @sa getNetArmor()
	 */
	float getAttackSpeedBonusA() const;
	
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
	
public: /* protected */
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
	
private:
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
	
	/** @name production related */
	//@{
	void clearProductionQueue() { this->m_production.queue.clear(); }
	void addToProductionQueue(ObjectId_t what) { this->m_production.queue.push_back(what); }
	ObjectId_t firstSlotInProductionQueue()
	{
		if(this->m_production.queue.empty())
			return ObjectId::None;
		else
			return this->m_production.queue.front();
	}
	void popFirstSlotInProductionQueue() { this->m_production.queue.pop_front(); }
	//@}
	
	//////// Now it's data part
	
	/** @name Object owner/state/position etc. */
	//@{
	Game *m_game;
	Player *m_owner;
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
	
	UnitAction::ActionTable m_actions;
	
	/** Production related data */
	struct ms_production
	{
		std::deque<ObjectId_t> queue; /**< production queue */
		double starttime; /** the time when the first slot in the queue has begun */
	} m_production;
	
	//std::list<UpgradeId> m_upgrade_queue; /**< Currently not used; todo: implement this */
	//std::deque<UnitOrder> m_order_queue; /**< Currently not used; todo: implement this */
	
	//RenderingContext *m_renderctx; /**< TODO: contains information about rendering states. animation frame etc. It's not used in Object class */
	
	/////////////////////////////////////////////////////////////////////////
public:
	/** @name Constant object attributes */
	//@{
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
	
	/** @brief Get object's initial state.
	 *  @sa ObjectState
	 */
	ObjectState_t getInitialState() const { return this->m_constattrs.initial_state; }
	int getMaxHP() const { return this->m_constattrs.max_hp; }
	int getMaxEnergy() const { return this->m_constattrs.max_energy; }
	int getInitialMinerals() const { return this->m_constattrs.initial_minerals; }
	int getInitialVespeneGas() const { return this->m_constattrs.initial_vespene_gas; }
	int getProvidedSupplies() const { return this->m_constattrs.provided_supplies; }
	int getRequiredSupplies() const { return this->m_constattrs.required_supplies; }
	
	/** @brief Get bare armor of the object.
	 *  @detail DO NOT use this function unless you want the RAW unit attribute(without upgrades).
	 *  @detail use getNetArmor() instead.
	 */
	float getArmor() const { return this->m_constattrs.armor; }
	/** @brief Get bare damage of the object.
	 *  @detail DO NOT use this function unless you want the RAW unit attribute(without upgrades).
	 *  @detail use getNetDamage() instead.
	 */
	float getDamage() const { return this->m_constattrs.damage; }
	/** @brief Get bare moving speed of the object.
	 *  @detail DO NOT use this function unless you want the RAW unit attribute(without upgrades).
	 *  @detail use getNetMovingSpeed() instead.
	 */
	float getMovingSpeed() const { return this->m_constattrs.moving_speed; }
	/** @brief Get bare attack speed of the object.
	 *  @detail DO NOT use this function unless you want the RAW unit attribute(without upgrades).
	 *  @detail use getNetAttackSpeed() instead.
	 */
	float getAttackSpeed() const { return this->m_constattrs.attack_speed; }
	/** @brief Get bare attack range of the object.
	 *  @detail DO NOT use this function unless you want the RAW unit attribute(without upgrades).
	 *  @detail use getNetAttackRange() instead.
	 */
	float getAttackRange() const { return this->m_constattrs.attack_range; }
	//@}
	
public:
	/** @brief Creates a new object based on this object.
	 *  @return Pointer to newly created object.
	 *  @sa SC::ObjectFactory
	 */
	ObjectPtr clone();
	
protected:
	/** @name Constant object attributes */
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
	
private:
	bool m_cleanup_called; /**< for debugging */
};


} // end of namespace SC

#define END_OF_SCObject_H_
#endif

