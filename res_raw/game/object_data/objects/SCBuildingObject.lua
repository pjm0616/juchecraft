--[[
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
--]]

SCObjects.Building = setmetatable({
	object_type = ObjectType.Building;
	object_id = ObjectId.Building;
	object_id_name = "Building";
	object_name = "Building";
}, {__index = SCObjects.Object})

object_prototypes[#object_prototypes + 1] = SCObjects.Building




