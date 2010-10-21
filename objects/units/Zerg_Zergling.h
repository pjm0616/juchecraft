// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitObject_Zerg_Zergling_H_
#define SCUnitObject_Zerg_Zergling_H_

namespace SC {
namespace Objects {
namespace Units {

class Zerg_Zergling: public UnitObject
{
public:
	Zerg_Zergling(Game *game)
		: UnitObject(game)
	{
		this->m_object_type = ObjectType::Unit;
		this->m_object_id = ObjectId::Zerg_Zergling;
		this->m_object_id_name = "Zerg_Zergling";
		this->m_object_name = "Zerg Zergling";
		this->m_race_id = RaceId::Zerg;
		
		this->m_width = 16;
		this->m_height = 16;
		this->m_max_hp = 50;
		
		this->m_supplies_in_use = 1;
		this->m_damage = 2.5;
		this->m_moving_speed = 30.0;
		this->m_attack_speed = 15.0;
		this->m_attack_range = 10.0;
	}
	virtual ~Zerg_Zergling() {}
	
protected:
	
private:
	
};

} // end of namespace Units
} // end of namespace Objects
} // end of namespace SC

#endif

