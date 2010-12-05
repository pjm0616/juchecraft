/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
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

