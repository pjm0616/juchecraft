
object_prototypes[#object_prototypes + 1] = setmetatable({
	object_id = ObjectId.Terran_CommandCenter;
	object_id_name = "Terran_CommandCenter";
	object_name = "Terran Command Center";
	race_id = RaceId.Terran;
		
	width = 117;
	height = 83;
		
	max_hp = 1500;
	armor = 1;
		
	provided_supplies = 9;
}, {__index = SCObjects.Building})




