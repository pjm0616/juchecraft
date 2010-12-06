--[[
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
--]]


object_prototypes = {}

local object_data_base_path = './'

local function dirname(path)
	return path:gsub('[^/]*$', '')
end
local function basename(path)
	return path:gmatch('([^/]*)$')()
end

local function loaddata(arg1, arg2)
	local dir, file
	
	if arg2 == nil then
		dir = dirname(arg1)
		file = basename(arg1)
	else
		dir = arg1
		file = arg2
	end
	
	local prev_base = object_data_base_path
	object_data_base_path = object_data_base_path .. dir
	
	local res
	if type(file) == 'table' then
		for _, entry in pairs(file) do
			if type(entry) == 'table' then
				loaddata(entry[1], entry[2])
			else
				res = dofile(object_data_base_path .. entry .. '.lua')
			end
		end
	else
		res = dofile(object_data_base_path .. file .. '.lua')
	end
	
	object_data_base_path = prev_base
	
	return res
end


loaddata('types')
SCObjects = {}
local object_list = loaddata('SCObjectList')
loaddata('', object_list)


loaddata('data_dumper')
local s = DataDumper(object_prototypes)
--io.write(string.dump(s))
io.write(s)


return object_prototypes


