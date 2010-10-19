
#if !defined(SCObject_H_) || defined(END_OF_SCObject_H_)
#error You must include "SCObject.h"
#error Do not use this file directly.
#endif


// abccc
// a: SCRace
// b: SCObjectType
// ccc: id
enum
{
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
	
	Juche_AojiWorker = 52001, // 아오지 탄광 노동자
	Juche_DaepodongLauncher = 52002, // 대포동 발사병
	Juche_RodongdangPartisan = 52003, // 로동당원
	Juche_SecretAgent = 52004, // 간첩
	
	Juche_Hero_KimIlseong = 52101, 
	Juche_Hero_KimJeongil = 52102, 

	Juche_JoseonRodongdang = 53001, // 조선로동당
	Juche_RodongCorrectionalFacility = 53002,  // 로동교화소 - 자원채취
	Juche_JeongilStatue = 53003, // 김정일 동상 - 영웅 생산
	Juche_JucheTower = 53004, // 주체사상탑 - 유닛 업그레이드
	Juche_NuclearReactor = 53005, // 원자로
	Juche_DeactivatedNuclearReactor = 53006, // 냉각탑 폭파된 원자로 - 하나 지을때마다 서플라이 8 증가
	Juche_School = 53007, // 강제 소집 학교
};




