
object_prototypes[#object_prototypes + 1] = setmetatable({
	object_id = ObjectId.Terran_SCV;
	object_id_name = "Terran_SCV";
	object_name = "Terran SCV";
	race_id = RaceId.Terran;
		
	width = 23;
	height = 23;
		
	required_supplies = 1;
	damage = 5.0;
	moving_speed = 20.0;
	attack_speed = 2.0;x
}, {__index = SCObjects.Unit})




