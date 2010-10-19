// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCResourceObject_MineralField_H_
#define SCResourceObject_MineralField_H_

namespace SC {
namespace Objects {
namespace Resources {

class MineralField: public ResourceObject
{
public:
	MineralField(Game *game)
		: ResourceObject(game)
	{
		this->object_type = ObjectType::Resource;
		this->object_id = ObjectId::Resource_MineralField;
		this->object_id_name = "Resource_MineralField";
		this->object_name = "Mineral Field";
		
		this->width = 32;
		this->height = 32;
		
		this->initial_resource = 1500;
	}
	virtual ~MineralField() {}
	
protected:
	
private:
	
};

} // end of namespace Resources
} // end of namespace Objects
} // end of namespace SC

#endif

