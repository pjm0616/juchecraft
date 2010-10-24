
object_prototypes[#object_prototypes + 1] = setmetatable({
	object_id = ObjectId.Juche_AojiWorker;
	object_id_name = "Juche_AojiWorker";
	object_name = "주체 아오지 탄광 노동자";
	race_id = RaceId.Juche;
		
	width = 23;
	height = 22;
	max_hp = 50;
		
	required_supplies = 1;
	damage = 6.0;
	moving_speed = 25.0;
	attack_speed = 10.0;
}, {__index = SCObjects.Unit})




