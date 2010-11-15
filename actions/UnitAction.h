// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitAction_H_
#define SCUnitAction_H_

namespace SC {
namespace UnitAction {

/** An abstract class for UnitAction::*
 */
class Action: public WeakPtrOwner<Action>
{
public:
	// initAction() must be called after ctor
	Action(ActionId_t actid = ActionId::None);
	virtual ~Action();
	ObjectPtr getObject() { return this->m_obj.lock(); }
	
	ActionId_t getActionId() const { return this->m_actid; }
	
	// you must call setObject() in initAction
	virtual bool initAction(const ObjectPtr &obj) = 0; // { this->setObject(obj); return true; }
	virtual bool process(float time) = 0;
	
	bool isFinished() const { return this->m_is_finished; }
	bool isStarted() const { return this->m_is_started; }
protected:
	void setAsFinished(bool onoff = true) { this->m_is_finished = onoff; }
	void setAsStarted(bool onoff = true) { this->m_is_started = onoff; }
	
	// initAction() calls setObject()
	void setObject(const ObjectPtr &obj) { this->m_obj = obj; }
	
	ActionId_t m_actid;
	ObjectWeakPtr m_obj;
	
private:
	bool m_is_finished, m_is_started;
};



} /* END OF namespace UnitAction */
} /* END OF namespace SC */

#endif

