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
	
	/** Processes production job
	 */
	void process();
	
private:
	/** Returns the owner of the class
	 */
	inline Object *getObject() { return this->m_obj; }
	
	/** Create produced unit to the field.
	 */
	void do_produce(const ProductionInfoPtr &prodinfo);
	
private:
	Object *m_obj; /**< owner of the object(UnitProductionManager) */
	std::deque<ProductionInfoPtr> m_prodqueue; /**< production queue */
};


} // end of namespace SC

#define END_OF_SCObject_H_
#endif

