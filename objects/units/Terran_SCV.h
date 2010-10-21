// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitObject_Terran_SCV_H_
#define SCUnitObject_Terran_SCV_H_

namespace SC {
namespace Objects {
namespace Units {

class Terran_SCV: public UnitObject
{
public:
	Terran_SCV(Game *game)
		: UnitObject(game)
	{
		this->m_object_type = ObjectType::Unit;
		this->m_object_id = ObjectId::Terran_SCV;
		this->m_object_id_name = "Terran_SCV";
		this->m_object_name = "Terran SCV";
		this->m_race_id = RaceId::Terran;
		
		this->m_width = 23;
		this->m_height = 23;
		
		this->m_supplies_in_use = 1;
		this->m_damage = 5.0;
		this->m_moving_speed = 20.0;
		this->m_attack_speed = 2.0;
	}
	virtual ~Terran_SCV() {}
	
protected:
	
private:
	
};

} // end of namespace Units
} // end of namespace Objects
} // end of namespace SC

#endif

