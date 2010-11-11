// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitAction_H_
#define SCUnitAction_H_

namespace SC {


/** An abstract class for UnitAction_*
 */
class UnitAction
{
public:
	/** Warning: when calling this function, you must call setObject immediately
	 */
	UnitAction(UnitActionId_t actid = UnitActionId::None);
	UnitAction(const ObjectPtr &obj, UnitActionId_t actid);
	virtual ~UnitAction();
	
	UnitActionPtr makeThisPtr();
	UnitActionPtr getPtr() const { return UnitActionPtr(this->m_this); }
	
	/** the action is activated when you call this function
	 */
	void setObject(const ObjectPtr &obj);
	ObjectPtr getObject() const { return ObjectPtr(this->m_obj); }
	
	UnitActionId_t getActionId() const { return this->m_actid; }
	
	virtual bool process(float time) {return false;}
	
	bool isActivated() const { return this->m_is_activated; }
protected:
	void setAsActivated(bool onoff = true) { this->m_is_activated = onoff; }
	
	SC::weak_ptr<UnitAction> m_this;
	
	UnitActionId_t m_actid;
	ObjectWeakPtr m_obj;
	
private:
	bool m_is_activated;
};




} /* END OF namespace SC */

#endif

