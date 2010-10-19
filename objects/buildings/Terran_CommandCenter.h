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
		this->object_type = ObjectType::Unit;
		this->object_id = ObjectId::Terran_CommandCenter;
		this->object_id_name = "Terran_CommandCenter";
		this->object_name = "Terran Command Center";
		this->race_id = RaceId::Terran;
		
		this->width = 117;
		this->height = 83;
		
		this->supplied_food = 9;
	}
	virtual ~Terran_CommandCenter() {}
	
protected:
	
private:
	
};

} // end of namespace Buildings
} // end of namespace Objects
} // end of namespace SC

#endif

