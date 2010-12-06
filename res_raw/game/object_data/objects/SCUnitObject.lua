--[[
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
--]]

SCObjects.Unit = setmetatable({
	object_type = ObjectType.Unit;
	object_id = ObjectId.Unit;
	object_id_name = "Unit";
	object_name = "Unit";
}, {__index = SCObjects.Object})

object_prototypes[#object_prototypes + 1] = SCObjects.Unit




