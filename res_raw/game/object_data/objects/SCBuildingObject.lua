
SCObjects.Building = setmetatable({
	object_type = ObjectType.Building;
	object_id = ObjectId.Building;
	object_id_name = "Building";
	object_name = "Building";
}, {__index = SCObjects.Object})

object_prototypes[#object_prototypes + 1] = SCObjects.Building




