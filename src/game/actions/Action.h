/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/actions/Action.h
**/

#ifndef SCUnitAction_Action_H_
#define SCUnitAction_Action_H_

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

} /* END OF namespace UnitAction */
} /* END OF namespace SC */

#endif

