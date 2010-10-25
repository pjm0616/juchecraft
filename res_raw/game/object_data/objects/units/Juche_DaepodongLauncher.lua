
object_prototypes[#object_prototypes + 1] = setmetatable({
	object_id = ObjectId.Juche_DaepodongLauncher;
	object_id_name = "Juche_DaepodongLauncher";
	object_name = "주체 대포동발사병";
	race_id = RaceId.Juche;
		
	width = 17;
	height = 20;
	max_hp = 50;
		
	required_supplies = 1;
	damage = 8.0;
	moving_speed = 20.0;
	attack_speed = 4.0;
	attack_range = 70.0;
}, {__index = SCObjects.Unit})




