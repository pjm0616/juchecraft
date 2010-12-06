--[[
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
--]]

SCObjects.Resource = setmetatable({
	object_type = ObjectType.Resource;
	object_id = ObjectId.Resource;
	object_id_name = "Resource";
	object_name = "Resource";
}, {__index = SCObjects.Object})

object_prototypes[#object_prototypes + 1] = SCObjects.Resource




