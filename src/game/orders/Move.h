// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitOrder_Move_H_
#define SCUnitOrder_Move_H_

namespace SC {
namespace UnitOrder {

class Move: public TargetedOrder
{
public:
	
	OrderId_t getOrderID() const { return this->m_cmdid; }
	
	virtual bool initOrder(const ObjectPtr &obj) = 0;
	virtual bool process(const ObjectPtr &obj, float time) = 0;
	
private:
	OrderId_t m_cmdid;
	const OrderInfo *m_info;
};


} /* END OF namespace UnitOrder */
} /* END OF namespace SC */

#endif

