// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitObject_H_
#define SCUnitObject_H_

namespace SC {


class UnitObject: public Object
{
public:
	UnitObject(Game *game)
		: Object(game)
	{
		this->m_object_type = ObjectType::Unit;
		this->m_object_id = ObjectId::Unit;
		this->m_object_id_name = "Unit";
		this->m_object_name = "Unit";
	}
	virtual ~UnitObject() {}
	
	
private:
	
};


} // end of namespace SC

#endif

