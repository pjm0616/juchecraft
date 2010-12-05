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

class ProductionInfo
{
public:
	ObjectId_t m_objid;
	time_t m_time;
};
typedef shared_ptr<ProductionInfo> ProductionInfoPtr;


class ProductionOrder: public Order
{
public:
	ProductionOrder();
	ProductionOrder(const ProductionInfoPtr &info);
	virtual ~ProductionOrder();
	
	virtual bool initOrder(const ObjectPtr &obj);
	virtual bool process(float deltat);
	virtual OrderPtr clone(OrderPtr cloned_order = null_order);
	
	
	
private:
	ProductionInfoPtr m_prodinfo;
};


} /* END OF namespace UnitOrder */
} /* END OF namespace SC */

#endif

