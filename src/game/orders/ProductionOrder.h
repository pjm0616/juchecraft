/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/orders/ProductionOrder.h
**/

#ifndef SCUnitOrder_ProductionOrder_H_
#define SCUnitOrder_ProductionOrder_H_

namespace SC {
namespace UnitOrder {

class ProductionOrder: public Order
{
public:
	ProductionOrder(OrderId_t orderid = OrderId::None);
	virtual ~ProductionOrder();
	
	virtual bool initOrder(const ObjectPtr &obj);
	virtual bool process(float deltat);
	virtual OrderPtr clone(OrderPtr cloned_order = null_order);
	
	
	
private:
	ObjectId_t m_obj_to_be_produced;
};


} /* END OF namespace UnitOrder */
} /* END OF namespace SC */

#endif

