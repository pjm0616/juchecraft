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
	/** Warning: when calling this function, you must call setObject immediately
	 */
	Action(ActionId_t actid = ActionId::None);
	virtual ~Action();
	
	ActionId_t getActionId() const { return this->m_actid; }
	
	virtual bool initAction(const ObjectPtr &obj) = 0;
	virtual bool process(const ObjectPtr &obj, float time) = 0;
	
	bool isFinished() const { return this->m_is_finished; }
	bool isStarted() const { return this->m_is_started; }
protected:
	void setAsFinished(bool onoff = true) { this->m_is_finished = onoff; }
	void setAsStarted(bool onoff = true) { this->m_is_started = onoff; }
	
	ActionId_t m_actid;
	
private:
	bool m_is_finished, m_is_started;
};



} /* END OF namespace UnitAction */
} /* END OF namespace SC */

#endif

