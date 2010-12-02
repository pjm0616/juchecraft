/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/actions/TargetedAction.h
**/

#ifndef SCUnitAction_TargetedAction_H_
#define SCUnitAction_TargetedAction_H_

namespace SC {
namespace UnitAction {


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

