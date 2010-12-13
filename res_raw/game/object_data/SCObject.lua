--[[
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *	
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *	
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *	
 *	See LICENSE.txt for details.
--]]

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


