// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitObject_Juche_AojiWorker_H_
#define SCUnitObject_Juche_AojiWorker_H_

namespace SC {
namespace Objects {
namespace Units {

class Juche_AojiWorker: public UnitObject
{
public:
	Juche_AojiWorker(Game *game)
		: UnitObject(game)
	{
		this->m_object_type = ObjectType::Unit;
		this->m_object_id = ObjectId::Juche_AojiWorker;
		this->m_object_id_name = "Juche_AojiWorker";
		this->m_object_name = "Juche AojiWorker";
		this->m_race_id = RaceId::Juche;
		
		this->m_width = 23;
		this->m_height = 22;
		this->m_max_hp = 50;
		
		this->m_supplies_in_use = 1;
		this->m_damage = 6.0;
		this->m_moving_speed = 25.0;
		this->m_attack_speed = 10.0;
	}
	virtual ~Juche_AojiWorker() {}
	
protected:
	
private:
	
};

} // end of namespace Units
} // end of namespace Objects
} // end of namespace SC

#endif

