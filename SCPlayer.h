// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCPlayer_H_
#define SCPlayer_H_

namespace SC {


namespace RaceId
{
	enum
	{
		None = 0, 
		Neutral = 1, 

		Terran = 2, 
		Protoss = 3, 
		Zerg = 4, 

		Juche = 5, 
		
		Size // number of RaceId
	};
}
typedef unsigned int RaceId_t;


class Player
{
public:
	enum { MAX_PLAYER = 8 };
	static Player Players[Player::MAX_PLAYER + 1];
	static void initialize();
	enum
	{
		NeutralPlayer = 0, 
		Player1 = 1, Player2, Player3, Player4, 
		Player5, Player6, Player7, Player8
	};
	
	Player();
	Player(int player_id) { this->setPlayerId(player_id); }
	~Player() {}
	
	int getPlayerId() const { return this->player_id; }
	unsigned int getPlayerColor() const { return this->player_color; }
	
	RaceId_t getRaceId() const { return this->race_id; }
	
	int getMinerals() const { return this->minerals; }
	void setMinerals(int minerals) { this->minerals = minerals; }
	void increaseMinerals(int val) { this->minerals += val; }
	void decreaseMinerals(int val) { this->minerals -= val; }
	
	int getVespeneGas() const { return this->vespene_gas; }
	void setVespeneGas(int vespene_gas) { this->vespene_gas = vespene_gas; }
	void increseVespeneGas(int val) { this->vespene_gas += val; }
	void decreseVespeneGas(int val) { this->vespene_gas -= val; }
	
	//const int *getFoodMax() const { return this->food_max; }
	int getFoodMax(RaceId_t race) const { return this->food_max[race]; }
	void setFoodMax(RaceId_t race, int food_max) { this->food_max[race] = food_max; }
	int increaseFoodMax(RaceId_t race, int val) { return (this->food_max[race] += val); }
	int decreaseFoodMax(RaceId_t race, int val) { return (this->food_max[race] -= val); }
	
	//const int *getFoodCrnt() const { return this->food_crnt; }
	int getFoodCrnt(RaceId_t race) const { return this->food_crnt[race]; }
	void setFoodCrnt(RaceId_t race, int food_crnt) { this->food_crnt[race] = food_crnt; }
	int increaseFoodCrnt(RaceId_t race, int val) { return (this->food_crnt[race] += val); }
	int decreaseFoodCrnt(RaceId_t race, int val) { return (this->food_crnt[race] -= val); }
	
	float getPlayerArmorBonusA() const { return this->added_armor_bonus; }
	float getPlayerDamageBonusA() const { return this->added_damage_bonus; }
	float getPlayerMovingSpeedBonusA() const { return this->added_moving_speed_bonus; }
	float getPlayerAttackSpeedBonusA() const { return this->added_attack_speed_bonus; }
	
//protected:
	void setRace(RaceId_t race) { this->race_id = race; }
	
private:
	static bool is_initialized;
	
	int player_id;
	unsigned int player_color;
	
	RaceId_t race_id;
	
	int minerals, vespene_gas;
	int food_max[RaceId::Size], food_crnt[RaceId::Size];
	
	float added_armor_bonus, added_damage_bonus, added_moving_speed_bonus, added_attack_speed_bonus;
	
	void setPlayerId(int player_id) { this->player_id = player_id; }
	void setPlayerColor(unsigned int player_color) { this->player_color = player_color; }
	
	void setPlayerArmorBonusA(float v) { this->added_armor_bonus = v; }
	void setPlayerDamageBonusA(float v) { this->added_damage_bonus = v; }
	void setPlayerMovingSpeedBonusA(float v) { this->added_moving_speed_bonus = v; }
	void setPlayerAttackSpeedBonusA(float v) { this->added_attack_speed_bonus = v; }
};



} // end of namespace SC

#endif

