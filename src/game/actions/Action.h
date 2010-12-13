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

/** @file src/game/actions/Action.h
**/

#ifndef SCUnitAction_Action_H_
#define SCUnitAction_Action_H_

namespace SC {
namespace UnitAction {

struct ProcessResult
{
	enum
	{
		Continue, 
		Finished, 
	};
};
typedef unsigned int ProcessResult_t;

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
	virtual ProcessResult_t process(float time) = 0;
	
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

