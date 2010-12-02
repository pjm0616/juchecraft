/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/orders/TargetedOrder.h
**/

#ifndef SCUnitOrder_TargetedOrder_H_
#define SCUnitOrder_TargetedOrder_H_

namespace SC {
namespace UnitOrder {

class TargetedOrder: public Order
{
public:
	TargetedOrder(OrderId_t orderid = OrderId::None);
	TargetedOrder(const Target &target, OrderId_t orderid = OrderId::None);
	virtual ~TargetedOrder();
	
	virtual bool initOrder(const ObjectPtr &obj);
	virtual bool process(float deltat);
	virtual OrderPtr clone(OrderPtr cloned_order = null_order);
	
	void clearTarget() { this->m_target.clear(); }
	void setTarget(const Target &target) { this->m_target = target; }
	const Target &getTarget() const { return this->m_target; }
	
private:
	Target m_target;
};


} /* END OF namespace UnitOrder */
} /* END OF namespace SC */

#endif

