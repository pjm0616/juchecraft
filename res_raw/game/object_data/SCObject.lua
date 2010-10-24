
SCObjects.Object = {
	object_type = ObjectType.None;
	object_id = ObjectId.None;
	object_id_name = "Object";
	object_name = "Object";
	race_id = RaceId.None;
	
	initial_state = ObjectState.None;
	width = 0;
	height = 0;
	max_hp = -1;
	max_energy = -1;
	initial_minerals = 0;
	initial_vespene_gas = 0;
	provided_supplies = 0;
	required_supplies = 0;
	armor = 0.0;
	damage = 0.0;
	moving_speed = 0.0;
	attack_speed = 0.0;
	attack_range = 0.0;
}

object_prototypes[#object_prototypes + 1] = SCObjects.Object


