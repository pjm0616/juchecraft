// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCBuildingObject_H_
#define SCBuildingObject_H_

namespace SC {


class BuildingObject: public Object
{
public:
	BuildingObject(Game *game)
		: Object(game)
	{
		this->m_object_type = ObjectType::Building;
		this->m_object_id = ObjectId::Building;
		this->m_object_id_name = "Building";
		this->m_object_name = "Building";
	}
	virtual ~BuildingObject() {}
	
	
private:
	
};


} // end of namespace SC

#endif

