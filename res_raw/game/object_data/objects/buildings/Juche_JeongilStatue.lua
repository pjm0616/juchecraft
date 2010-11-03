
object_prototypes[#object_prototypes + 1] = setmetatable({
	object_id = ObjectId.Juche_JeongilStatue;
	object_id_name = "Juche_JeongilStatue";
	object_name = "주체 김정일 동상";
	race_id = RaceId.Juche;
	
	width = 60;
	height = 120;
		
	max_hp = 800;
	armor = 1;
}, {__index = SCObjects.Building})




