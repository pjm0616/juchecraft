/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/orders/targeted/Order_Attack.h
**/

#ifndef SCUnitOrder_Attack_H_
#define SCUnitOrder_Attack_H_

namespace SC {
namespace UnitOrder {

class Attack: public TargetedOrder
{
	typedef TargetedOrder super;
public:
	Attack();
	Attack(const Target &target);
	
	virtual bool initOrder(const ObjectPtr &obj);
	virtual ProcessResult_t process(float time);
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

