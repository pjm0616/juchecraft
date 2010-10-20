// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitObject_Zerg_Zergling_H_
#define SCUnitObject_Zerg_Zergling_H_

namespace SC {
namespace Objects {
namespace Units {

class Zerg_Zergling: public UnitObject
{
public:
	Zerg_Zergling(Game *game)
		: UnitObject(game)
	{
		this->object_type = ObjectType::Unit;
		this->object_id = ObjectId::Zerg_Zergling;
		this->object_id_name = "Zerg_Zergling";
		this->object_name = "Zerg Zergling";
		this->race_id = RaceId::Zerg;
		
		this->width = 16;
		this->height = 16;
		this->max_hp = 50;
		
		this->supplies_in_use = 1;
		this->damage = 2.5;
		this->moving_speed = 30.0;
		this->attack_speed = 15.0;
		this->attack_range = 10.0;
	}
	virtual ~Zerg_Zergling() {}
	
protected:
	
private:
	
};

} // end of namespace Units
} // end of namespace Objects
} // end of namespace SC

#endif

