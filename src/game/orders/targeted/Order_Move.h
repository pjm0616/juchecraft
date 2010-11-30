// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitOrder_Move_H_
#define SCUnitOrder_Move_H_

namespace SC {
namespace UnitOrder {

class Move: public TargetedOrder
{
public:
	Move();
	Move(const Target &target);
	
	virtual bool initOrder(const ObjectPtr &obj);
	virtual bool process(float time);
	virtual OrderPtr clone(OrderPtr cloned_order = null_order);
	
private:
	/** @name state information
	 */
	//@{
	struct
	{
		unsigned int step;
	} m_state;
	//@}
};


} /* END OF namespace UnitOrder */
} /* END OF namespace SC */

#endif
