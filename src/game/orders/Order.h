/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *	
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *	
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/orders/Order.h
**/

#ifndef SCUnitOrder_Order_H_
#define SCUnitOrder_Order_H_

namespace SC {
namespace UnitOrder {

struct ProcessResult
{
	enum
	{
		Continue, 
		Finished, 
	};
};
typedef unsigned int ProcessResult_t;

static OrderPtr null_order ATTRIBUTE_UNUSED;


/** @brief An abstract class that describes unit orders
 *  @detail
 *  This class describes the constant information about the Orders that user orders.
 *  For details, see Order.
 *  @sa Order
 */
class OrderInfo
{
public:
	OrderInfo(OrderId_t orderid)
		: m_orderid(orderid)
	{
	}
	~OrderInfo()
	{
	}
	
	OrderId_t getOrderID() const { return this->m_orderid; }
	
private:
	OrderId_t m_orderid;
	
	/** The requirements in order to execute this Order
	 */
	OrderRequirementsPtr m_reqirements;
	
	/** A list of orders that will be executed sequentially.
	 */
	//std::vector<UnitAction> m_actionlist;
};

/** @brief An abstract class that describes unit orders
 *  @detail
 *  This class describes Orders that user orders.
 *  Basically a Order consists of a list of actions, Order parameters, and 
 *  required resources, upgrades, buildings, etc.
 *  Constant information is stored in OrderInfo class.
 *  Once a Order is assigned to an object, 
 *  firstly the object checks if the conditions mets with the requirements.
 *  Then the actions specified in m_actionlist will be sequentially executed.
 *  When the final action is finished, the whole Order is finished and, 
 *  SC::Object will execute next Order in the queue.
 *  @sa OrderInfo
 */
class Order
{
public:
	Order(OrderId_t orderid = OrderId::None);
	virtual ~Order();
	
	OrderId_t getOrderID() const { return this->m_orderid; }
	ObjectPtr getObject() { return this->m_obj.lock(); }
	
	/** Initializes order.
	 *  @return true if succeeded, false if failed.
	 *  @return returning false will cancel the order.
	 *  @sa Object::doOrder()
	 */
	virtual bool initOrder(const ObjectPtr &obj);
	/** Processes order.
	 *  @return if the order has finished, return false and the order will be removed
	 *  @return otherwise return true
	 *  @sa Object::processOrder()
	 */
	virtual ProcessResult_t process(float deltat);
	/** Clones the order.
	 *  This method only clones order types and parameters, not order states.
	 */
	virtual OrderPtr clone(OrderPtr cloned_order = null_order);
	
	bool isPrimaryOrder() const { return this->m_rank == 0; }
	//void isSecondaryOrder() const { return this->m_rank == 1; }
	
	bool isFinished() const { return this->m_is_finished; }
	bool isStarted() const { return this->m_is_started; }
protected:
	void setAsFinished(bool onoff = true) { this->m_is_finished = onoff; }
	void setAsStarted(bool onoff = true) { this->m_is_started = onoff; }
	
	/** only to be used by derived class' ctor
	 *  defaults to primary order
	 *  somewhat dirty
	 */
	void setAsPrimaryOrder() { this->m_rank = 0; }
	void setAsSecondaryOrder() { this->m_rank = 1; }
	
	const OrderInfo *m_info; /**< Information of the order(requirements, etc.) */
	
	/** recursively calls base class' clone() and returns a pointer to cloned order.
	 *  Only to be called by Order*::clone()
	 */
	template<class This_, class Base_>
	inline This_ *do_clone_head(OrderPtr &cloned_order)
	{
		if(!cloned_order)
			cloned_order.reset(new This_);
		dynastatic_cast<Base_ *>(this)->Base_::clone(cloned_order);
		
		return dynastatic_cast<This_ *>(cloned_order.get());
	}
	
private:
	OrderId_t m_orderid;
	ObjectWeakPtr m_obj;
	/** 0(primary, foreground) or 1(secondary, background)
	 *  @sa Object::m_secondary_order
	 */
	int m_rank;
	
	/** @name state information
	 */
	//@{
	bool m_is_finished, m_is_started;
	//@}
};


/** This order does nothing
 */
class NoOrder: public Order
{
public:
	virtual ProcessResult_t process(float deltat) {return ProcessResult::Finished;}
};


} /* END OF namespace UnitOrder */
} /* END OF namespace SC */

#endif

