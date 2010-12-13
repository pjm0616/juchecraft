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

/** @file src/game/actions/TargetedAction.h
**/

#ifndef SCUnitAction_TargetedAction_H_
#define SCUnitAction_TargetedAction_H_

namespace SC {
namespace UnitAction {


class TargetedAction: public Action
{
	typedef Action super;
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

