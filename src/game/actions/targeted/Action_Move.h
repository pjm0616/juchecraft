/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/actions/targeted/Action_Move.h
**/

#ifndef SCUnitAction_Move_H_
#define SCUnitAction_Move_H_

namespace SC {
namespace UnitAction {

class Move: public TargetedAction
{
	typedef TargetedAction super;
public:
	/** @brief Defines movement options.
	 *  @details This is an enum class. C++98 does not support enum classes, damn.
	 *  TODO: remove MovementFlags in action class, move it to UnitOrder::Attack
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
	
	Move(const Target &target, float minimum_distance = 0.0, MovementFlags_t flags = MovementFlags::None);
	virtual ~Move();
	
	virtual bool initAction(const ObjectPtr &obj);
	virtual ProcessResult_t process(float time);
private:
	void cleanup();
	
	//@{
	void setMinimumDistanceToTarget(float mindist) { this->m_min_distance_to_target = mindist; }
	float getMinimumDistanceToTarget() const { return this->m_min_distance_to_target; }
	
	void setStartPoint(const Coordinate &pos) { this->m_start_point = pos; }
	const Coordinate &getStartPoint() const { return this->m_start_point; }
	//@}
	
	Coordinate calculateDestination_TargetedMoving();
	Coordinate calculateSpeed(float time);
	
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
	// m_target != this
	ObjectPtr m_target;
	
	// if movement_target is set and movement_min_distance_to_target is set, 
	//		object moves to movement_target.getPosition(), but keeps distance `min. movement_min_distance_to_target' to target.
	float m_min_distance_to_target; // TODO: implement this
};




} /* END OF namespace UnitAction */
} /* END OF namespace SC */

#endif

