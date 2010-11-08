// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitAction_H_
#define SCUnitAction_H_

namespace SC {



namespace UnitActionId
{
	enum UnitActionId
	{
		None, 
		Move, 
		Attack, 
	};
}

class UnitAction
{
public:
	UnitAction(ObjectSPtr_t *obj);
	virtual ~UnitAction();
	
	virtual bool process(float time) {return false;}
private:
	ObjectSPtr_t *m_obj;
	UnitActionId_t m_actid;
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
	
	UnitAction_Move(ObjectSPtr_t *obj, const Coordinate &dest, MovementFlags_t flags = MovementFlags::None);
	UnitAction_Move(ObjectSPtr_t *obj, const ObjectSPtr_t &target, float minumum_distance = 0.0, MovementFlags_t flags = MovementFlags::None);
	
	virtual bool process(float time);
private:
	bool move(const Coordinate &dest, MovementFlags_t flags = MovementFlags::None);
	bool move(const ObjectSPtr_t &target, float minumum_distance = 0.0, MovementFlags_t flags = MovementFlags::None);
	
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
	void stopMoving();
	//@}
	
	//@{
	void setFinalDestination(const Coordinate &pos) { this->m_final_destination = pos; }
	Coordinate &getFinalDestination() { return this->m_final_destination; }
	const Coordinate &getFinalDestination() const { return this->m_final_destination; }
	void setDestination(const Coordinate &pos);
	Coordinate &getDestination() { return this->m_destination; }
	const Coordinate &getDestination() const { return this->m_destination; }
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

