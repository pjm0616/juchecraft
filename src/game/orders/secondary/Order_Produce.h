/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/orders/Order_Produce.h
**/

#ifndef SCUnitOrder_Produce_H_
#define SCUnitOrder_Produce_H_

namespace SC {
namespace UnitOrder {

class Produce: public Order
{
	typedef Order super;
public:
	Produce(ProductionInfoPtr info = ProductionInfoPtr());
	virtual ~Produce();
	
	virtual bool initOrder(const ObjectPtr &obj);
	virtual ProcessResult_t process(float deltat);
	virtual OrderPtr clone(OrderPtr cloned_order = null_order);
	
private:
	ProductionInfoPtr m_prodinfo;
};


} /* END OF namespace UnitOrder */
} /* END OF namespace SC */

#endif

