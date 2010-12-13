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

return {
		'SCObject', 
		{'objects/', {
			'SCBuildingObject', 
			{'buildings/', {
				'Terran_CommandCenter', 
				'Juche_RodongCorrectionalFacility', 
			}}, -- end of buildings
			'SCUnitObject', 
			{'units/', {
				'Terran_Marine', 
				'Terran_SCV', 
				'Zerg_Zergling', 
				'Juche_AojiWorker', 
				'Juche_DaepodongLauncher', 
			}}, -- end of units
			'SCResourceObject', 
			{'resources/', {
				'MineralField', 
			}}, -- end of resources
		}}, -- end of objects
	}



