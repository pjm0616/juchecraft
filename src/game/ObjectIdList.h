/*
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
**/

/** @file src/game/ObjectIdList.h
**/

#ifndef SCObjectIdList_H_
#define SCObjectIdList_H_

namespace SC {

namespace ObjectId
{
	enum ObjectId
	{
		// abccc
		// a: SCRace
		// b: SCObjectType
		// ccc: id
	
		None = 0, 
		Resource = 1001, 
		Unit = 2001, 
		Building = 3001, 
	
		Resource_MineralField = 11001, 
		Resource_VespeneGeyser = 11002, 
		Critter_Simyeong = 12001, 
		Critter_Airjang = 12001, 

		Terran_SCV = 22001, 
		Terran_Marine = 22002, 
		Terran_Medic = 22003, 
		Terran_Firebat = 22004, 

		Terran_CommandCenter = 23001, 
		Terran_SupplyDepot = 23002, 
		Terran_Barracks = 23003, 
		Terran_Academy = 23004, 
		Terran_Armory = 23005, 

		Protoss_Probe = 32001, 
		Protoss_Zealot = 32002, 
		Protoss_Dragoon = 32003, 
		Protoss_DarkTemplar = 32004, 

		Protess_Nexus = 33001, 
		Protoss_Pylon = 33002, 
		Protoss_Gateway = 33003, 
		Protoss_Forge = 33004, 
		Protoss_TemplarArchives = 33005, 

		Zerg_Larva = 42001, 
		Zerg_Drone = 42002, 
		Zerg_Zergling = 42003, 
		Zerg_Hydralisk = 42004, 
		Zerg_Untralisk = 42005, 

		Zerg_Hatchery = 43001, 
		Zerg_SpawningPool = 43002, 
		Zerg_CreepColony = 43003, 
		Zerg_HydraliskDen = 43004, 
	
		Juche_AojiWorker = 52001, 
		Juche_DaepodongLauncher = 52002, 
		Juche_RodongdangPartisan = 52003, 
		Juche_SecretAgent = 52004, 
	
		Juche_Hero_KimIlseong = 52101, 
		Juche_Hero_KimJeongil = 52102, 

		Juche_JoseonRodongdang = 53001, 
		Juche_RodongCorrectionalFacility = 53002, 
		Juche_JeongilStatue = 53003, 
		Juche_JucheTower = 53004, 
		Juche_NuclearReactor = 53005, 
		Juche_DeactivatedNuclearReactor = 53006, 
		Juche_School = 53007, 


	}; /* END OF enum ObjectId */
} /* END OF namespace ObjectId */

} /* END OF namespace SC */


#endif

