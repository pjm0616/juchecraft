
object_prototypes[#object_prototypes + 1] = setmetatable({
	object_id = ObjectId.Juche_NuclearReactor;
	object_id_name = "Juche_NuclearReactor";
	object_name = "주체 원자로";
	race_id = RaceId.Juche;
	
	width = 120;
	height = 80;
		
	max_hp = 2000;
	armor = 1;
}, {__index = SCObjects.Building})




