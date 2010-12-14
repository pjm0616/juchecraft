
object_prototypes[#object_prototypes + 1] = setmetatable({
	object_id = ObjectId.Terran_SCV;
	object_id_name = "Terran_SCV";
	object_name = "Terran SCV";
	race_id = RaceId.Terran;
		
	width = 23;
	height = 23;
	max_hp = 50;
		
	required_supplies = 1;
	damage = 5.0;
	moving_speed = 30.0;
	attack_speed = 2.0;
	attack_range = 18.0;
}, {__index = SCObjects.Unit})




