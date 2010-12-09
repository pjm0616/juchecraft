/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/UnitProductionManager.h
**/

#ifndef SC_Game_UnitProductionManager_H_
#define SC_Game_UnitProductionManager_H_

namespace SC {



// FIXME: move to elsewhere
class ProductionInfo
{
	friend class UnitProductionManager;
public:
	ProductionInfo()
		: m_objid(ObjectId::None)
		, m_time(0)
	{
		this->m_state.started_time = 0.0;
	}
	
	ProductionInfoPtr clone() const
	{
		ProductionInfo *o = new ProductionInfo;
		
		o->m_requirements = this->m_requirements;
		o->m_objid = this->m_objid;
		o->m_time = this->m_time;
		o->m_state.started_time = 0.0;
		
		return ProductionInfoPtr(o);
	}
	
private:
	bool isStarted() const { return this->m_state.started_time != 0.0; }
	void setStartedTime(double time) { this->m_state.started_time = time; }
	
public:
	OrderRequirementsConstPtr m_requirements;
	ObjectId_t m_objid;
	time_t m_time;
private:
	struct
	{
		double started_time;
	} m_state;
};




class UnitProductionManager
{
public:
	UnitProductionManager(Object *owner);
	~UnitProductionManager();
	
	void clearProductionQueue();
	void addToProductionQueue(const ProductionInfoPtr &what);
	void popFirstSlotInProductionQueue();
	
	bool isQueueEmpty() const { return this->m_prodqueue.empty(); }
	size_t numOfQueuedUnits() const { return this->m_prodqueue.size(); }
	
	ProductionInfoPtr &firstSlotInProductionQueue() { return this->m_prodqueue.front(); }
	const ProductionInfoPtr &firstSlotInProductionQueue() const { return this->m_prodqueue.front(); }
	
	void process();
	
private:
	inline Object *getObject() { return this->m_obj; }
	void do_produce(const ProductionInfoPtr &prodinfo);
	
private:
	Object *m_obj;
	std::deque<ProductionInfoPtr> m_prodqueue; /**< production queue */
};


} // end of namespace SC

#define END_OF_SCObject_H_
#endif

