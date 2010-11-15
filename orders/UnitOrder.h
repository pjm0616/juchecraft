// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitOrder_H_
#define SCUnitOrder_H_

namespace SC {
namespace UnitOrder {


/** @brief An abstract class that describes unit orders
 *  @detail
 *  This class describes the constant information about the Orders that user orders.
 *  For details, see Order.
 *  @sa Order
 */
class OrderInfo
{
public:
	OrderInfo(OrderId_t cmdid)
		: m_cmdid(cmdid)
	{
	}
	~OrderInfo()
	{
	}
	
	OrderId_t getOrderID() const { return this->m_cmdid; }
	
private:
	OrderId_t m_cmdid;
	
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
	Order()
	{
	}
	Order(OrderId_t cmdid)
		: m_cmdid(cmdid)
	{
	}
	~Order()
	{
	}
	
	OrderId_t getOrderID() const { return this->m_cmdid; }
	
	virtual bool initOrder(const ObjectPtr &obj) = 0;
	virtual bool process(const ObjectPtr &obj, float time) = 0;
	
private:
	OrderId_t m_cmdid;
	const OrderInfo *m_info;
};

class TargetedOrder: public Order
{
public:

private:
	
	struct
	{
		int type; // coordinate or object
		Coordinate coord;
		ObjectPtr obj;
	} m_target;
};


} /* END OF namespace UnitOrder */
} /* END OF namespace SC */

#endif

