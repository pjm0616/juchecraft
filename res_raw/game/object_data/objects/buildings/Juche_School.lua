
object_prototypes[#object_prototypes + 1] = setmetatable({
	object_id = ObjectId.Juche_School;
	object_id_name = "Juche_School";
	object_name = "주체 학교";
	race_id = RaceId.Juche;
	
	width = 100;
	height = 60;
		
	max_hp = 800;
	armor = 0;
}, {__index = SCObjects.Building})




