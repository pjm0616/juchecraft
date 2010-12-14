
object_prototypes[#object_prototypes + 1] = setmetatable({
	object_id = ObjectId.Terran_Marine;
	object_id_name = "Terran_Marine";
	object_name = "Terran Marine";
	race_id = RaceId.Terran;
		
	width = 17;
	height = 20;
	max_hp = 50;
		
	required_supplies = 1;
	damage = 5.0;
	moving_speed = 40.0;
	attack_speed = 8.0;
	attack_range = 70.0;
}, {__index = SCObjects.Unit})




