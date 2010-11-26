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
	/**
	 *  WARNING: initAction() MUST be called immediately after the ctor call.
	 *  Otherwise it may cause memory leak or something.
	 */
	Action(ActionId_t actid = ActionId::None);
	virtual ~Action();
	
	ActionId_t getActionId() const { return this->m_actid; }
	ObjectPtr getObject() { return this->m_obj.lock(); }
	
	// you must call setObject() in initAction
	virtual bool initAction(const ObjectPtr &obj);
	virtual bool process(float time) = 0;
	
	bool isFinished() const { return this->m_is_finished; }
	bool isStarted() const { return this->m_is_started; }
protected:
	void setAsFinished(bool onoff = true) { this->m_is_finished = onoff; }
	void setAsStarted(bool onoff = true) { this->m_is_started = onoff; }
	
	// initAction() calls setObject()
	void setObject(const ObjectPtr &obj) { this->m_obj = obj; }
	
private:
	ActionId_t m_actid;
	ObjectWeakPtr m_obj;
	
	bool m_is_finished, m_is_started;
};

class TargetedAction: public Action
{
public:
	TargetedAction(ActionId_t actid = ActionId::None);
	TargetedAction(const Target &target, ActionId_t actid = ActionId::None);
	virtual ~TargetedAction();
	
	bool setTarget(const Target &target);
	void clearTarget() { this->m_target.clear(); }
	const Target &getTarget() const { return this->m_target; }
	
	virtual bool initAction(const ObjectPtr &obj);
	
private:
	Target m_target; // assert(m_target != this->getObject());
};


} /* END OF namespace UnitAction */
} /* END OF namespace SC */

#endif

