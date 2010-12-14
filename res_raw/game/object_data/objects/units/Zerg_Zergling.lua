
object_prototypes[#object_prototypes + 1] = setmetatable({
	object_id = ObjectId.Zerg_Zergling;
	object_id_name = "Zerg_Zergling";
	object_name = "Zerg Zergling";
	race_id = RaceId.Zerg;
		
	width = 16;
	height = 16;
	max_hp = 50;
		
	required_supplies = 1;
	damage = 2.5;
	moving_speed = 45.0;
	attack_speed = 11.0;
	attack_range = 18.0;
}, {__index = SCObjects.Unit})




