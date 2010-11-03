
object_prototypes[#object_prototypes + 1] = setmetatable({
	object_id = ObjectId.Juche_JucheTower;
	object_id_name = "Juche_JucheTower";
	object_name = "주체 주체사상탑";
	race_id = RaceId.Juche;
	
	width = 60;
	height = 120;
		
	max_hp = 600;
	armor = 0;
}, {__index = SCObjects.Building})




