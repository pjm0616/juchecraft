
object_prototypes[#object_prototypes + 1] = setmetatable({
	object_id = ObjectId.Juche_DeactivatedNuclearReactor;
	object_id_name = "Juche_DeactivatedNuclearReactor";
	object_name = "주체 냉각탑이 폭파된 원자로";
	race_id = RaceId.Juche;
	
	width = 120;
	height = 80;
		
	max_hp = 800;
	armor = 0;
	
	provided_supplies = 8;
}, {__index = SCObjects.Building})




