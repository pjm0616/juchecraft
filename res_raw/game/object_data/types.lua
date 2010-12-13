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

local function tostring_tblkey(tbl, key)
	return key
end

ObjectState = setmetatable({}, {__index = tostring_tblkey})

ObjectType = 
{

	None = 0, 
	Resource = 1, 
	Unit = 2, 
	Building = 3, 
}

RaceId = 
{
	None = 0, --/**< Map objects, resources, bullets, etc. */
	Neutral = 1, --/**< Neutral units */

	Terran = 2, 
	Protoss = 3, 
	Zerg = 4, 

	Juche = 5, 

	Size --/**< Number of RaceId entries */
}

ObjectId = 
{
	-- abccc
	-- a: SCRace
	-- b: SCObjectType
	-- ccc: id

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

	Juche_AojiWorker = 52001, -- 아오지 탄광 노동자
	Juche_DaepodongLauncher = 52002, -- 대포동 발사병
	Juche_RodongdangPartisan = 52003, -- 로동당원
	Juche_SecretAgent = 52004, -- 간첩

	Juche_Hero_KimIlseong = 52101, 
	Juche_Hero_KimJeongil = 52102, 

	Juche_JoseonRodongdang = 53001, -- 조선로동당
	Juche_RodongCorrectionalFacility = 53002,  -- 로동교화소 - 자원채취
	Juche_JeongilStatue = 53003, -- 김정일 동상 - 영웅 생산
	Juche_JucheTower = 53004, -- 주체사상탑 - 유닛 업그레이드
	Juche_NuclearReactor = 53005, -- 원자로
	Juche_DeactivatedNuclearReactor = 53006, -- 냉각탑 폭파된 원자로 - 하나 지을때마다 서플라이 8 증가
	Juche_School = 53007, -- 강제 소집 학교


}



