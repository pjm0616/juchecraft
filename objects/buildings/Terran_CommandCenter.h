// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCBuildingObject_Terran_CommandCenter_H_
#define SCBuildingObject_Terran_CommandCenter_H_

namespace SC {
namespace Objects {
namespace Buildings {

class Terran_CommandCenter: public BuildingObject
{
public:
	Terran_CommandCenter(Game *game)
		: BuildingObject(game)
	{
		this->m_object_type = ObjectType::Unit;
		this->m_object_id = ObjectId::Terran_CommandCenter;
		this->m_object_id_name = "Terran_CommandCenter";
		this->m_object_name = "Terran Command Center";
		this->m_race_id = RaceId::Terran;
		
		this->m_width = 117;
		this->m_height = 83;
		
		this->m_max_hp = 1500;
		this->m_armor = 1;
		
		this->m_supplied_food = 9;
	}
	virtual ~Terran_CommandCenter() {}
	
protected:
	
private:
	
};

} // end of namespace Buildings
} // end of namespace Objects
} // end of namespace SC

#endif

