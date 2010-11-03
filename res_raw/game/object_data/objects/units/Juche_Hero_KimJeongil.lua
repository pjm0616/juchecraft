
object_prototypes[#object_prototypes + 1] = setmetatable({
	object_id = ObjectId.Juche_Hero_KimJeongil;
	object_id_name = "Juche_Hero_KimJeongil";
	object_name = "주체 김정일";
	race_id = RaceId.Juche;
		
	width = 20;
	height = 20;
	max_hp = 200;
	armor = 0;
		
	required_supplies = 4;
	damage = 1.0;
	moving_speed = 10.0;
	attack_speed = 1.0;
	attack_range = 40.0;
}, {__index = SCObjects.Unit})




