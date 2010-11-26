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
	
	Player(Game *game);
	~Player();
	
	int getPlayerId() const { return this->m_player_id; }
	unsigned int getPlayerColor() const { return this->m_player_color; }
	
	RaceId_t getRaceId() const { return this->m_race_id; }
	
	/** @brief Set player's race
	 *  @param[in] race Player's new race
	 */
	void setRace(RaceId_t race) { this->m_race_id = race; }
	
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
	
	//@{
	/** @brief Get selected objects.
	 *  @return A constant reference to ObjectList.
	 */
	const ObjectList &getSelectedObjs() const { return this->m_selected_objs; }
	//ObjectList &getSelectedObjsForWriting() { return this->m_selected_objs; }
	
	/** @brief Checks if the object is selecetd.
	 *  @param[in] obj The object to check if selected
	 *  @return true if selected, otherwise false
	 */
	bool isSelectedObject(const ObjectPtr &obj) const;
	
	/** @brief clears selected object list
	 */
	void clearSelectedObjectList() { this->m_selected_objs.clear(); }
	
	struct SelectionFlags
	{
		enum
		{
			SET, /**< Clears selected object list and set newly selected objects. */
			ADD, /**< Adds newly selected objects to selected objects list */
			REMOVE, /**< Removes newly selected objects from selected objects list */
		};
	};
	typedef unsigned int SelectionFlags_t;
	/** @brief Starts object selection
	 */
	void startObjectSelection(const Coordinate &start_coord);
	
	/** @brief Finished object selection, and updates selected object list.
	 *  @return The number of selected objects
	 */
	size_t finishObjectSelection(const Coordinate &end_coord, SelectionFlags_t flags = SelectionFlags::SET);
	
	/** @brief Get Currently selected objects
	 *  @detail Don't call this function if isSelectionInProgress() == false
	 *  @detail results are stored in `buf'
	 *  @param[out] buf A referece to ObjectList to store the results in.
	 *  @return The number of selected objects.
	 *  @sa SelectionFlags
	 */
	size_t getCurrentlySelectedObjects(ObjectList &buf, const Coordinate &crnt_coord, SelectionFlags_t flags) const;
	
	/** @brief Selects object that are inside the rect[`coord' ~ `coord2']
	 *  @detail internally this function calls startObjectSelection, finishObjectSelection continuously
	 *  @return The number of selected objects.
	 *  @sa SelectionFlags
	 */
	size_t selectObjects(const Coordinate &coord1, const Coordinate &coord2, SelectionFlags_t flags = SelectionFlags::SET);
	
	/** @brief Checks if object selection is in progress
	 */
	bool isSelectionInProgress() const { return this->m_selection_in_progress; }
	/** @brief Get the coordinate where the selection was started.
	 */
	const Coordinate &getSelectionStartCoordinate() const { return this->m_selection_start_coordinate; }
	//@}
	
	/** set player's current order
	 */
	void setOrder(const UnitOrder::OrderPtr &order);
	void setOrder(UnitOrder::Order *order) { this->setOrder(UnitOrder::OrderPtr(order)); }
	/** clear player's current order
	 */
	void clearOrder();
	const UnitOrder::OrderPtr &getOrder() const { return this->m_order; }
	
	/** set player's current order to currently selected objects and
	 *  clears player's current order (calls clearOrder())
	 */
	void multiDoCurrentOrder();
	/** clear orders of currently selected objects and
	 *  clears player's current order (calls clearOrder())
	 */
	void multiCancelOrder();
	
	/** Sets target of player's current order. (specifically TargetedOrder)
	 */
	void setOrderTargetByCoord(const Coordinate &coord);
	
	// order queue related
	#if 0
	void addToOrderQueue(const UnitOrder::OrderPtr &cmd) { this->m_orderqueue.push_back(cmd); }
	void addToOrderQueue(UnitOrder::Order *cmd) { this->m_orderqueue.push_back(UnitOrder::OrderPtr(cmd)); }	// be careful when using this function
	void clearOrderQueue() { this->m_orderqueue.clear(); }
	const UnitOrder::OrderPtr &getFirstOrderInQueue() const;
	void popFirstOrderInQueue() { this->m_orderqueue.pop_front(); }
	bool isOrderQueueEmpty() const { return this->m_orderqueue.empty(); }
	#endif
	
	void toast(const std::string &msg, time_t duration = -1); // FIXME: dirty hack; see Player.cpp
	
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
	
	Game *m_game;
	int m_player_id;
	unsigned int m_player_color;
	RaceId_t m_race_id;
	
	int m_minerals, m_vespene_gas;
	int m_crnt_supplies[RaceId::Size], m_supplies_in_use[RaceId::Size];
	
	float m_added_armor_bonus, m_added_damage_bonus, m_added_moving_speed_bonus, m_added_attack_speed_bonus;
	
private:
	/** filters objects in the list according to internal rules
	 *  TODO: move to ObjectFilter.h
	 *  @param[in] select_cnt_limit Maximum number of object to select.
	 */
	void filterCurSelectedObjects(ObjectList &selected_objs, int select_cnt_limit) const;
	/** @brief Merges two object list
	 *  @param[in,out] orig The ObjectList to merge in
	 *  @param[in] newobjs A constant referenct to ObjectList to merge
	 *  @param[in] flags merge options
	 *  @sa SelectionFlags
	 */
	static void mergeObjectList(ObjectList &orig, const ObjectList &newobjs, SelectionFlags_t flags);
	
	bool m_selection_in_progress; /**< true if object selection is in progress */
	Coordinate m_selection_start_coordinate; /**< The coordinate where the selection was started. Only set if m_selection_in_progress is true */
	ObjectList m_selected_objs; /**< The list of selected objects */
	
private:
	UnitOrder::OrderPtr m_order;
	//std::deque<UnitOrder::OrderPtr> m_orderqueue;
};



} // end of namespace SC

#endif

