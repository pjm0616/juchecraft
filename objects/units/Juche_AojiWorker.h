// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUnitObject_Juche_AojiWorker_H_
#define SCUnitObject_Juche_AojiWorker_H_

namespace SC {
namespace Objects {
namespace Units {

class Juche_AojiWorker: public UnitObject
{
public:
	Juche_AojiWorker(Game *game)
		: UnitObject(game)
	{
		this->object_type = ObjectType::Unit;
		this->object_id = ObjectId::Juche_AojiWorker;
		this->object_id_name = "Juche_AojiWorker";
		this->object_name = "주체 아오지 탄광 노동자";
		this->race_id = RaceId::Juche;
		
		this->width = 23;
		this->height = 22;
		
		this->supplies_in_use = 1;
		this->damage = 6.0;
		this->moving_speed = 25.0;
		this->attack_speed = 10.0;
	}
	virtual ~Juche_AojiWorker() {}
	
protected:
	
private:
	
};

} // end of namespace Units
} // end of namespace Objects
} // end of namespace SC

#endif

