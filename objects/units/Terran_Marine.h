// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitObject_Terran_Marine_H_
#define SCUnitObject_Terran_Marine_H_

namespace SC {
namespace Objects {
namespace Units {

class Terran_Marine: public UnitObject
{
public:
	Terran_Marine(Game *game)
		: UnitObject(game)
	{
		this->object_type = ObjectType::Unit;
		this->object_id = ObjectId::Terran_Marine;
		this->object_id_name = "Terran_Marine";
		this->object_name = "Terran Marine";
		this->race_id = RaceId::Terran;
		
		this->width = 17;
		this->height = 20;
		this->max_hp = 50;
		
		this->supplies_in_use = 1;
		this->damage = 5.0;
		this->moving_speed = 20.0;
		this->attack_speed = 10.0;
		this->attack_range = 50.0;
	}
	virtual ~Terran_Marine() {}
	
protected:
	
private:
	
};

} // end of namespace Units
} // end of namespace Objects
} // end of namespace SC

#endif

