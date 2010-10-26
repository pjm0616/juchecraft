// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

/** @file SCPlayer.h
 *  @brief Game player object
**/



#ifndef SCPlayer_H_
#define SCPlayer_H_

namespace SC {


class Player
{
public:
	// move these enums to somewhere else
	enum { MAX_PLAYER = 8 };
	enum
	{
		NeutralPlayer = 0, 
		Player1 = 1, Player2, Player3, Player4, 
		Player5, Player6, Player7, Player8
	};
	
	Player();
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
	/** @brief Get the amount of currently provided supplies of all races
	 *  @return An array of integers that contains values. Use RaceId_t as array index.
	 */
	const int *getCurrentSupplies() const { return this->m_crnt_supplies; }
	/** @brief Get the amount of currently provided supplies of a race
	 *  @param[in] race The race to get value from
	 *  @return The amount of provided supplies
	 */
	int getCurrentSupplies(RaceId_t race) const { return this->m_crnt_supplies[race]; }
	/** @brief Set the amount of currently provided supplies of a race
	 *  @param[in] race The race to set value
	 *  @param[in] val New amount of provided supplies
	 */
	void setCurrentSupplies(RaceId_t race, int val) { this->m_crnt_supplies[race] = val; }
	/** @brief Increase the amount of currently provided supplies of a race
	 *  @param[in] race The race to set value
	 *  @param[in] val The amount to increase provided supplies
	 */
	void increaseCurrentSupplies(RaceId_t race, int val) { this->m_crnt_supplies[race] += val; }
	/** @brief Decrease the amount of currently provided supplies of a race
	 *  @param[in] race The race to set value
	 *  @param[in] val The amount to decrease provided supplies
	 */
	void decreaseCurrentSupplies(RaceId_t race, int val) { this->m_crnt_supplies[race] -= val; }
	//@}
	
	//@{
	/** @brief Get the amount of currently being used supplies of all races
	 *  @return An array of integers that contains values. Use RaceId_t as array index.
	 */
	const int *getSuppliesInUse() const { return this->m_supplies_in_use; }
	/** @brief Get the amount of currently being used supplies of a race
	 *  @param[in] race The race to get value from
	 *  @return The amount of currently being used supplies
	 */
	int getSuppliesInUse(RaceId_t race) const { return this->m_supplies_in_use[race]; }
	/** @brief Set the amount of currently being used supplies of a race
	 *  @param[in] race The race to set value
	 *  @param[in] val New amount of currently being used supplies
	 */
	void setSuppliesInUse(RaceId_t race, int val) { this->m_supplies_in_use[race] = val; }
	/** @brief Increase the amount of currently provided supplies of a race
	 *  @param[in] race The race to set value
	 *  @param[in] val The amount to increase provided supplies
	 */
	void increaseSuppliesInUse(RaceId_t race, int val) { this->m_supplies_in_use[race] += val; }
	/** @brief Decrease the amount of currently provided supplies of a race
	 *  @param[in] race The race to set value
	 *  @param[in] val The amount to decrease provided supplies
	 */
	void decreaseSuppliesInUse(RaceId_t race, int val) { this->m_supplies_in_use[race] -= val; }
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
	
	const ObjectList &getSelectedObjs() const { return this->m_selected_objs; }
	//ObjectList &getSelectedObjsForWriting() { return this->m_selected_objs; }
	bool isSelectedObject(const ObjectSPtr_t &obj) const;
	void clearSelectedObjectList() { this->m_selected_objs.clear(); }
	struct SelectFlags
	{
		enum
		{
			SET, 
			ADD, 
			DEL
		};
	};
	typedef unsigned int SelectFlags_t;
	size_t selectObjects(const Coordinate &coord1, const Coordinate &coord2, SelectFlags_t flags = SelectFlags::SET);
	
//protected:
	/** @brief Set player's race
	 *  @param[in] race Player's new race
	 */
	void setRace(RaceId_t race) { this->m_race_id = race; }
	
public:
	/** @brief set player id/color
	 *  @details these functions are ONLY used by SC::Game
	 */
	//@{
	void setPlayerId(int player_id) { this->m_player_id = player_id; }
	void setPlayerColor(unsigned int player_color) { this->m_player_color = player_color; }
	//@}
	
	void setPlayerArmorBonusA(float v) { this->m_added_armor_bonus = v; }
	void setPlayerDamageBonusA(float v) { this->m_added_damage_bonus = v; }
	void setPlayerMovingSpeedBonusA(float v) { this->m_added_moving_speed_bonus = v; }
	void setPlayerAttackSpeedBonusA(float v) { this->m_added_attack_speed_bonus = v; }
	
private:
	
	int m_player_id;
	unsigned int m_player_color;
	RaceId_t m_race_id;
	
	int m_minerals, m_vespene_gas;
	int m_crnt_supplies[RaceId::Size], m_supplies_in_use[RaceId::Size];
	
	float m_added_armor_bonus, m_added_damage_bonus, m_added_moving_speed_bonus, m_added_attack_speed_bonus;
	
private:
	ObjectList m_selected_objs;
};



} // end of namespace SC

#endif

