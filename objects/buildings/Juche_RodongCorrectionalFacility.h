// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCBuildingObject_Juche_RodongCorrectionalFacility_H_
#define SCBuildingObject_Juche_RodongCorrectionalFacility_H_

namespace SC {
namespace Objects {
namespace Buildings {

class Juche_RodongCorrectionalFacility: public BuildingObject
{
public:
	Juche_RodongCorrectionalFacility(Game *game)
		: BuildingObject(game)
	{
		this->m_object_type = ObjectType::Unit;
		this->m_object_id = ObjectId::Juche_RodongCorrectionalFacility;
		this->m_object_id_name = "Juche_RodongCorrectionalFacility";
		this->m_object_name = "Juche RodongCorrectionalFacility";
		this->m_race_id = RaceId::Juche;
		
		this->m_width = 117;
		this->m_height = 83;
		
		this->m_supplied_food = 9;
	}
	virtual ~Juche_RodongCorrectionalFacility() {}
	
protected:
	
private:
	
};

} // end of namespace Buildings
} // end of namespace Objects
} // end of namespace SC

#endif

