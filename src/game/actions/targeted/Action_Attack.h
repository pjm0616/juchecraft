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

/** @file src/game/actions/targeted/Action_Attack.h
**/

#ifndef SCUnitAction_Attack_H_
#define SCUnitAction_Attack_H_

namespace SC {
namespace UnitAction {

class Attack: public TargetedAction
{
	typedef TargetedAction super;
public:
	Attack(const Target &target);
	virtual ~Attack();
	
	virtual bool initAction(const ObjectPtr &obj);
	virtual ProcessResult_t process(float time);
private:
	void cleanup();
	
	void setLastAttackTime(double time) { this->m_last_attack_time = time; }
	double getLastAttackTime() const { return this->m_last_attack_time; }
	
	double m_last_attack_time;
};




} /* END OF namespace UnitAction */
} /* END OF namespace SC */

#endif

