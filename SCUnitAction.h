// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitAction_H_
#define SCUnitAction_H_

namespace SC {


/** An abstract class for UnitAction_*
 */
class UnitAction: public WeakPtrOwner<UnitAction>
{
public:
	/** Warning: when calling this function, you must call setObject immediately
	 */
	UnitAction(UnitActionId_t actid = UnitActionId::None);
	virtual ~UnitAction();
	
	UnitActionId_t getActionId() const { return this->m_actid; }
	
	virtual bool process(const ObjectPtr &obj, float time) {return false;}
	
	bool isFinished() const { return this->m_is_finished; }
	bool isStarted() const { return this->m_is_started; }
protected:
	void setAsFinished(bool onoff = true) { this->m_is_finished = onoff; }
	void setAsStarted(bool onoff = true) { this->m_is_started = onoff; }
	
	UnitActionId_t m_actid;
	
private:
	bool m_is_finished, m_is_started;
};




} /* END OF namespace SC */

#endif

