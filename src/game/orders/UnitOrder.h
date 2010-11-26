// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitOrder_H_
#define SCUnitOrder_H_

namespace SC {
namespace UnitOrder {

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
	
	/** @brief The requirements in order to execute this Order
	 */
	struct ms_requirements
	{
		int hit_points;
		int shield;
		int energy;
		int minerals;
		int vespene_gas;
		int supplies;
		std::vector<ObjectId_t> objects;
		//std::vector<UpgradeId_t> upgrades;
	} m_requirements;
	/** @brief A list of orders that will be executed sequentially.
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
 *  firstly the object checks if the conditions complies with the requirements.
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
	
	virtual bool initOrder(const ObjectPtr &obj);
	virtual bool process(float deltat);
	virtual OrderPtr clone(OrderPtr cloned_order = null_order);
	
	bool isFinished() const { return this->m_is_finished; }
	bool isStarted() const { return this->m_is_started; }
protected:
	void setAsFinished(bool onoff = true) { this->m_is_finished = onoff; }
	void setAsStarted(bool onoff = true) { this->m_is_started = onoff; }
	
	const OrderInfo *m_info;
	
	template<class This_, class Base_>
	inline This_ *do_clone_head(OrderPtr &cloned_order)
	{
		if(!cloned_order)
			cloned_order.reset(new This_);
		dynamic_cast<Base_ *>(this)->Base_::clone(cloned_order);
		
		return dynamic_cast<This_ *>(cloned_order.get());
	}
	
private:
	OrderId_t m_orderid;
	ObjectWeakPtr m_obj;
	
	/** @name state information
	 */
	//@{
	bool m_is_finished, m_is_started;
	//@}
};

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


/** This order does nothing
 */
class NoOrder: public Order
{
public:
	virtual bool process(float deltat) {return true;}
};


} /* END OF namespace UnitOrder */
} /* END OF namespace SC */

#endif

