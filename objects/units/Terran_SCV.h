// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitObject_Terran_SCV_H_
#define SCUnitObject_Terran_SCV_H_

namespace SC {
namespace Objects {
namespace Units {

class Terran_SCV: public UnitObject
{
public:
	Terran_SCV(Game *game)
		: UnitObject(game)
	{
		this->object_type = ObjectType::Unit;
		this->object_id = ObjectId::Terran_SCV;
		this->object_id_name = "Terran_SCV";
		this->object_name = "Terran SCV";
		this->race_id = RaceId::Terran;
		
		this->width = 23;
		this->height = 23;
		
		this->supplies_in_use = 1;
		this->damage = 5.0;
		this->moving_speed = 20.0;
		this->attack_speed = 2.0;
	}
	virtual ~Terran_SCV() {}
	
protected:
	
private:
	
};

} // end of namespace Units
} // end of namespace Objects
} // end of namespace SC

#endif

