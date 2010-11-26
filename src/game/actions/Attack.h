// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitAction_Attack_H_
#define SCUnitAction_Attack_H_

namespace SC {
namespace UnitAction {

class Attack: public TargetedAction
{
public:
	Attack(const Target &target);
	virtual ~Attack();
	
	virtual bool initAction(const ObjectPtr &obj);
	virtual bool process(float time);
private:
	void cleanup();
	
	void setLastAttackTime(double time) { this->m_last_attack_time = time; }
	double getLastAttackTime() const { return this->m_last_attack_time; }
	
	double m_last_attack_time;
};




} /* END OF namespace UnitAction */
} /* END OF namespace SC */

#endif

