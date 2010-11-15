// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitOrder_H_
#define SCUnitOrder_H_

namespace SC {
namespace UnitOrder {


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

