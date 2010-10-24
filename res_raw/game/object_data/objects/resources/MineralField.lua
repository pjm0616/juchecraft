
object_prototypes[#object_prototypes + 1] = setmetatable({
	object_id = ObjectId.Resource_MineralField;
	object_id_name = "Resource_MineralField";
	object_name = "우라늄 탄광";
		
	initial_state = ObjectState.Invincible;
	
	--width = 32;
	--height = 32;
	-- grp size
	--width = 64;
	--height = 54;
	-- modified
	width = 64;
	height = 44;
		
	initial_minerals = 1500;
}, {__index = SCObjects.Resource})




