// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCResourceObject_H_
#define SCResourceObject_H_

namespace SC {


class ResourceObject: public Object
{
public:
	ResourceObject(Game *game)
		: Object(game)
	{
		this->object_type = ObjectType::Resource;
		this->object_id = ObjectId::Resource;
		this->object_id_name = "Resource";
		this->object_name = "Resource";
	}
	virtual ~ResourceObject() {}
	
	
private:
	
};



} // end of namespace SC

#endif

