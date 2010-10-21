// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

/** @file SCPlayer.h
 *  @brief Game player object
**/



#ifndef SCPlayer_H_
#define SCPlayer_H_

namespace SC {


/** @brief Defines race IDs.
 */
namespace RaceId
{
	typedef unsigned int RaceId_t;
	enum RaceId
	{
		None = 0, /**< Map objects, resources, bullets, etc. */
		Neutral = 1, /**< Neutral units */

		Terran = 2, 
		Protoss = 3, 
		Zerg = 4, 

		Juche = 5, 
		
		Size /**< Number of RaceId entries */
	};
}
using RaceId::RaceId_t;


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
	
	int getPlayerId() const { return this->m_player_id; }
	unsigned int getPlayerColor() const { return this->m_player_color; }
	
	RaceId_t getRaceId() const { return this->m_race_id; }
	
	//@{
	/** @brief Get the amount of player's minerals
	 *  @return The amount of player's minerals
	 */
	int getMinerals() const { return this->m_minerals; }
	/** @brief Set the amount of player's minerals
	 *  @param[in] val new amount of minerals
	 */
	void setMinerals(int val) { this->m_minerals = val; }
	/** @brief Increase the amount of player's minerals
	 *  @param[in] val amount of minerals to increase
	 */
	void increaseMinerals(int val) { this->m_minerals += val; }
	/** @brief Decrease the amount of player's minerals
	 *  @param[in] val amount of minerals to decrease
	 */
	void decreaseMinerals(int val) { this->m_minerals -= val; }
	//@}
	
	//@{
	/** @brief Get the amount of player's vespene gas
	 *  @return The amount of player's vespene gas
	 */
	int getVespeneGas() const { return this->m_vespene_gas; }
	/** @brief Set the amount of player's vespene gas
	 *  @param[in] val new amount of vespene gas
	 */
	void setVespeneGas(int val) { this->m_vespene_gas = val; }
	/** @brief Increase the amount of player's vespene gas
	 *  @param[in] val amount of vespene gas to increase
	 */
	void increseVespeneGas(int val) { this->m_vespene_gas += val; }
	/** @brief Decrease the amount of player's vespene gas
	 *  @param[in] val amount of vespene gas to decrease
	 */
	void decreseVespeneGas(int val) { this->m_vespene_gas -= val; }
	//@}
	
	//@{
	/** @brief Get the amount of currently supplied foods of all races
	 *  @return An array of integers that contains values. Use RaceId_t as array index.
	 */
	const int *getFoodMax() const { return this->m_food_max; }
	/** @brief Get the amount of currently supplied foods of a race
	 *  @param[in] race The race to get value from
	 *  @return The amount of supplied foods
	 */
	int getFoodMax(RaceId_t race) const { return this->m_food_max[race]; }
	/** @brief Set the amount of currently supplied foods of a race
	 *  @param[in] race The race to set value
	 *  @param[in] val New amount of supplied foods
	 */
	void setFoodMax(RaceId_t race, int val) { this->m_food_max[race] = val; }
	/** @brief Increase the amount of currently supplied foods of a race
	 *  @param[in] race The race to set value
	 *  @param[in] val The amount to increase supplied foods
	 */
	void increaseFoodMax(RaceId_t race, int val) { this->m_food_max[race] += val; }
	/** @brief Decrease the amount of currently supplied foods of a race
	 *  @param[in] race The race to set value
	 *  @param[in] val The amount to decrease supplied foods
	 */
	void decreaseFoodMax(RaceId_t race, int val) { this->m_food_max[race] -= val; }
	//@}
	
	//@{
	/** @brief Get the amount of currently used foods of all races
	 *  @return An array of integers that contains values. Use RaceId_t as array index.
	 */
	const int *getFoodCrnt() const { return this->m_food_crnt; }
	/** @brief Get the amount of currently used foods of a race
	 *  @param[in] race The race to get value from
	 *  @return The amount of currently used foods
	 */
	int getFoodCrnt(RaceId_t race) const { return this->m_food_crnt[race]; }
	/** @brief Set the amount of currently used foods of a race
	 *  @param[in] race The race to set value
	 *  @param[in] val New amount of currently used foods
	 */
	void setFoodCrnt(RaceId_t race, int val) { this->m_food_crnt[race] = val; }
	/** @brief Increase the amount of currently supplied foods of a race
	 *  @param[in] race The race to set value
	 *  @param[in] val The amount to increase supplied foods
	 */
	void increaseFoodCrnt(RaceId_t race, int val) { this->m_food_crnt[race] += val; }
	/** @brief Decrease the amount of currently supplied foods of a race
	 *  @param[in] race The race to set value
	 *  @param[in] val The amount to decrease supplied foods
	 */
	void decreaseFoodCrnt(RaceId_t race, int val) { this->m_food_crnt[race] -= val; }
	//@}
	
	//@{
	/** @brief Get added armor bonuses per player
	 *  @return The bonus value
	 */
	float getPlayerArmorBonusA() const { return this->m_added_armor_bonus; }
	/** @brief Get added damage bonuses per player
	 *  @return The bonus value
	 */
	float getPlayerDamageBonusA() const { return this->m_added_damage_bonus; }
	/** @brief Get added moving speed bonuses per player
	 *  @return The bonus value
	 */
	float getPlayerMovingSpeedBonusA() const { return this->m_added_moving_speed_bonus; }
	/** @brief Get added attack speed bonuses per player
	 *  @return The bonus value
	 */
	float getPlayerAttackSpeedBonusA() const { return this->m_added_attack_speed_bonus; }
	//@}
	
//protected:
	/** @brief Set player's race
	 *  @param[in] race Player's new race
	 */
	void setRace(RaceId_t race) { this->m_race_id = race; }
	
private:
	void setPlayerId(int player_id) { this->m_player_id = player_id; }
	void setPlayerColor(unsigned int player_color) { this->m_player_color = player_color; }
	
	void setPlayerArmorBonusA(float v) { this->m_added_armor_bonus = v; }
	void setPlayerDamageBonusA(float v) { this->m_added_damage_bonus = v; }
	void setPlayerMovingSpeedBonusA(float v) { this->m_added_moving_speed_bonus = v; }
	void setPlayerAttackSpeedBonusA(float v) { this->m_added_attack_speed_bonus = v; }
	
	static bool ms_is_initialized;
	
	int m_player_id;
	unsigned int m_player_color;
	RaceId_t m_race_id;
	
	int m_minerals, m_vespene_gas;
	int m_food_max[RaceId::Size], m_food_crnt[RaceId::Size];
	
	float m_added_armor_bonus, m_added_damage_bonus, m_added_moving_speed_bonus, m_added_attack_speed_bonus;
	
};



} // end of namespace SC

#endif

