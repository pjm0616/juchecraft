// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitAction_H_
#define SCUnitAction_H_

namespace SC {


/** An abstract class for UnitAction_*
 */
class UnitAction
{
public:
	/** Warning: when calling this function, you must call setObject immediately
	 */
	UnitAction(UnitActionId_t actid = UnitActionId::None);
	UnitAction(const ObjectSPtr_t &obj, UnitActionId_t actid);
	virtual ~UnitAction();
	
	void setObject(const ObjectSPtr_t &obj);
	const ObjectSPtr_t &getObject() const { return this->m_obj; }
	
	UnitActionId_t getActionId() const { return this->m_actid; }
	
	virtual bool process(float time) {return false;}
protected:
	UnitActionId_t m_actid;
	ObjectSPtr_t m_obj;
};


class UnitAction_Move: public UnitAction
{
public:
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
	
	UnitAction_Move(const ObjectSPtr_t &obj, const Coordinate &dest, MovementFlags_t flags = MovementFlags::None);
	UnitAction_Move(const ObjectSPtr_t &obj, const ObjectSPtr_t &target, float minimum_distance = 0.0, MovementFlags_t flags = MovementFlags::None);
	virtual ~UnitAction_Move(){}
	
	virtual bool process(float time);
private:
	bool move(const Coordinate &dest, MovementFlags_t flags = MovementFlags::None);
	bool move(const ObjectSPtr_t &target, float minimum_distance = 0.0, MovementFlags_t flags = MovementFlags::None);
	
	//@{
	void setMovement_MinimumDistanceToTarget(float distance) { this->m_min_distance_to_target = distance; }
	float getMovement_MinimumDistanceToTarget() const { return this->m_min_distance_to_target; }
	void setMovementTarget(const ObjectSPtr_t &target, float minimum_distance = 0.0)
		{ this->m_target = target; this->setMovement_MinimumDistanceToTarget(minimum_distance); }
	void clearMovementTarget() { this->m_target.reset(); }
	const ObjectSPtr_t &getMovementTarget() const { return this->m_target; }
	void setMovementStartPoint(const Coordinate &pos) { this->m_start_point = pos; }
	const Coordinate &getMovementStartPoint() const { return this->m_start_point; }
	
	Coordinate calculateDestination_TargetedMoving();
	Coordinate calculateMovementSpeed(float time);
	//@}
	
	//@{
	bool checkMinDistanceOld(const ObjectSPtr_t &target, float min_distance, Coordinate *where_to_move);
	/** @brief checks the distance between `this' and `dest'
	 *  @detail Checks if the distance if less(or equal) than `min_distance'.
	 *  @detail if not, stores the coordinate to move in order to attack target in `where_to_move'.
	 *  @return true if the distance if less(or equal) than `min_distance'. Otherwise false.
	 */
	bool checkMinDistance(const ObjectSPtr_t &target, float min_distance, Coordinate *where_to_move);
	//@}
	
	//@{
	void setFinalDestination(const Coordinate &pos) { this->m_final_destination = pos; }
	Coordinate &getFinalDestination() { return this->m_final_destination; }
	const Coordinate &getFinalDestination() const { return this->m_final_destination; }
	void setDestination(const Coordinate &pos);
	Coordinate &getDestination() { return this->m_destination; }
	const Coordinate &getDestination() const { return this->m_destination; }
	//@}
	
	//@{
	void setMovementFlags(MovementFlags_t val) { this->m_flags = val; }
	void setMovementFlags(MovementFlags_t type, bool onoff);
	MovementFlags_t getMovementFlags() const { return this->m_flags; }
	bool getMovementFlag(MovementFlags_t type) const { return this->getMovementFlags() & type; }
	//@}
	
	Coordinate m_start_point, m_destination, m_final_destination;
	MovementFlags_t m_flags;
	
	// if target is set, object moves to target. is target is not set, object moves to coordinate.
	ObjectSPtr_t m_target;
	
	// if movement_target is set and movement_min_distance_to_target is set, 
	//		object moves to movement_target.getPosition(), but keeps distance `min. movement_min_distance_to_target' to target.
	float m_min_distance_to_target; // TODO: implement this
};





} /* END OF namespace SC */

#endif

