// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitObject_Terran_Marine_H_
#define SCUnitObject_Terran_Marine_H_

namespace SC {
namespace Objects {
namespace Units {

class Terran_Marine: public UnitObject
{
public:
	Terran_Marine(Game *game)
		: UnitObject(game)
	{
		this->m_object_type = ObjectType::Unit;
		this->m_object_id = ObjectId::Terran_Marine;
		this->m_object_id_name = "Terran_Marine";
		this->m_object_name = "Terran Marine";
		this->m_race_id = RaceId::Terran;
		
		this->m_width = 17;
		this->m_height = 20;
		this->m_max_hp = 50;
		
		this->m_supplies_in_use = 1;
		this->m_damage = 5.0;
		this->m_moving_speed = 20.0;
		this->m_attack_speed = 10.0;
		this->m_attack_range = 50.0;
	}
	virtual ~Terran_Marine() {}
	
protected:
	
private:
	
};

} // end of namespace Units
} // end of namespace Objects
} // end of namespace SC

#endif

