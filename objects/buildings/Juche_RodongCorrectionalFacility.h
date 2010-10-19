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
		this->object_type = ObjectType::Unit;
		this->object_id = ObjectId::Juche_RodongCorrectionalFacility;
		this->object_id_name = "Juche_RodongCorrectionalFacility";
		this->object_name = "주체 로동교화소";
		this->race_id = RaceId::Juche;
		
		this->width = 117;
		this->height = 83;
		
		this->supplied_food = 9;
	}
	virtual ~Juche_RodongCorrectionalFacility() {}
	
protected:
	
private:
	
};

} // end of namespace Buildings
} // end of namespace Objects
} // end of namespace SC

#endif

