// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitAction_Move_H_
#define SCUnitAction_Move_H_

namespace SC {


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
	
	UnitAction_Move(const Coordinate &dest, MovementFlags_t flags = MovementFlags::None);
	UnitAction_Move(const ObjectPtr &target, float minimum_distance = 0.0, MovementFlags_t flags = MovementFlags::None);
	virtual ~UnitAction_Move(){}
	
	virtual bool process(const ObjectPtr &obj, float time);
private:
	/** this is the main action function. this action is activated in this function.
	 */
	//@{
	bool move(const Coordinate &dest, MovementFlags_t flags = MovementFlags::None);
	bool move(const ObjectPtr &target, float minimum_distance = 0.0, MovementFlags_t flags = MovementFlags::None);
	//@}
	
	//@{
	void setTarget(const ObjectPtr &target, float minimum_distance = 0.0);
	void clearTarget() { this->m_target.reset(); }
	const ObjectPtr &getTarget() const { return this->m_target; }
	float getMinimumDistanceToTarget() const { return this->m_min_distance_to_target; }
	
	void setStartPoint(const Coordinate &pos) { this->m_start_point = pos; }
	const Coordinate &getStartPoint() const { return this->m_start_point; }
	//@}
	
	Coordinate calculateDestination_TargetedMoving();
	Coordinate calculateSpeed(const ObjectPtr &obj, float time);
	
	//@{
	bool checkMinDistanceOld(const ObjectPtr &obj, const ObjectPtr &target, float min_distance, Coordinate *where_to_move);
	/** @brief checks the distance between `this' and `dest'
	 *  @detail Checks if the distance if less(or equal) than `min_distance'.
	 *  @detail if not, stores the coordinate to move in order to attack target in `where_to_move'.
	 *  @return true if the distance if less(or equal) than `min_distance'. Otherwise false.
	 */
	bool checkMinDistance(const ObjectPtr &obj, const ObjectPtr &target, float min_distance, Coordinate *where_to_move);
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
	// m_target != this
	ObjectPtr m_target;
	
	// if movement_target is set and movement_min_distance_to_target is set, 
	//		object moves to movement_target.getPosition(), but keeps distance `min. movement_min_distance_to_target' to target.
	float m_min_distance_to_target; // TODO: implement this
};





} /* END OF namespace SC */

#endif

