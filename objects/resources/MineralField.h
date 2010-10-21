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
		this->m_object_type = ObjectType::Resource;
		this->m_object_id = ObjectId::Resource_MineralField;
		this->m_object_id_name = "Resource_MineralField";
		this->m_object_name = "Mineral Field";
		
		this->m_initial_state = ObjectState::Invincible;
		this->m_width = 32;
		this->m_height = 32;
		
		this->m_initial_resource = 1500;
	}
	virtual ~MineralField() {}
	
protected:
	
private:
	
};

} // end of namespace Resources
} // end of namespace Objects
} // end of namespace SC

#endif

