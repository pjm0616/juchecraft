/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/Game.h
**/

#ifndef SCGame_H_
#define SCGame_H_

// FIXME: move this include directive to .cpp file
#include "ObjectFilter.h"


namespace SC {

class GameUI;
class Game
{
	friend class GameUI;
	
public:
	Game();
	~Game();
	
	/** Loads game data
	 */
	void loadGameData(const char *dir);
	/** Sets game UI
	 */
	void setUI(GameUI *ui) { this->m_ui = ui; }
	
	/** Game main loop
	 */
	void run();
	void test_tmp1(); /**< for debugging & testing */
	
	/** Creates a new object using ObjectFactory and `objid', and adds to game's object list
	 *  DO NOT call this function while iterating ObjectList.
	 *  @return a pointer to the newly created object.
	 */
	ObjectPtr newObject(ObjectId_t objid);
	/** Removes `obj' from the game.
	 */
	int removeObject(const ObjectPtr &obj);
	
	/** Finds object in the rect
	 *  @param [out] matched_objs a reference to ObjectList that will store the matched objects.
	 *  @return pointer to first object if found, otherwise NULL.
	 */
	ObjectPtr findObjectByRect(ObjectList &matched_objs, int left, int top, int right, int bottom, 
		unsigned int flags = ObjectFilter::Selectable);
	ObjectPtr findObjectByRect(ObjectList &matched_objs, const Coordinate &top_left, 
		const Coordinate &bottom_right, unsigned int flags = ObjectFilter::Selectable);
	
	/** Get current time from the system.
	 */
	double getElapsedTime() const;
	/** Get cached elapsed time.
	 *  Cached time will be updated at the end of update.
	 */
	double getCachedElapsedTime() const { return this->m_cached_elapsed_time; }
	inline float getDelta() const { return this->m_deltat; }
	inline float getFrameDelta() const { return this->m_frame_deltat; }
	unsigned int getFrameNumber() const { return this->m_frame_number; }
	unsigned int getUpdateRate() const { return 200; } // limit to 60 updates per second
	float getCurrentUpdateRate() const { return 1.0 / this->getDelta(); }
	float getCurrentFPS() const { return 1.0 / this->getFrameDelta(); }
	
	int getMapWidth() const { return this->m_map_width; }
	int getMapHeight() const { return this->m_map_height; }
	void getMapSize(int *width, int *height) { *width = this->getMapWidth(); *height = this->getMapHeight(); }
	// this function should be protected; map size will be set by calling loadMap()
	void setMapSize(int width, int height) { this->m_map_width = width; this->m_map_height = height; }
	
	// should be protected
	ObjectList &getObjectList() { return this->m_objects; }
	const ObjectList &getObjectList() const { return this->m_objects; }
	
	// Uncomment this if you really need a public accessor for m_ui.
	GameUI *getUI() { return this->m_ui; }
	
	void endGame() { this->m_is_game_ended = true; }
	inline bool isGameEnded() const { return this->m_is_game_ended; }
	
	Player *getPlayer(PlayerId_t player_id) const;
	const PlayerVector &getPlayers() const { return this->m_players; }
	
private:
	/** Adds an object
	 *  DO NOT call this function while iterating ObjectList.
	 *  @return `obj'
	 */
	const ObjectPtr &addObject(const ObjectPtr &obj);
	
	/** Removes all objects in the game (m_objects)
	 */
	void removeAllObjects();
	/** Removes all players in the game (m_players)
	 */
	void removeAllPlayers();
	
	/** Requests higher timer resoltion.
	 *  Only to be called by run()
	 */
	void startTimer();
	/** called when the game has finished
	 *  Only to be called by run()
	 */
	void endTimer();
	void setStartTime(time_t t) { this->m_start_time = t; }
	inline time_t getStartTime() const { return this->m_start_time; }
	inline void setCachedElapsedTime(double time) { this->m_cached_elapsed_time = time; }
	void setFrameNumber(unsigned int n) { this->m_frame_number = n; }
	inline void increaseFrameNumber(unsigned int n = 1) { this->m_frame_number += n; }
	inline void setDelta(float val) { this->m_deltat = val; }
	inline void setFrameDelta(float val) { this->m_frame_deltat = val; }
	void setLastDrawTime(double val) { this->m_last_draw = val; }
	inline double getLastDrawTime() const { return this->m_last_draw; }
	
	/** Iterates over object list and process each object
	 */
	void processObjects();
	
	GameUI *m_ui;
	ObjectFactory m_obj_factory;
	ObjectList m_objects;
	
	time_t m_start_time;
	double m_cached_elapsed_time; /**< Elapsed time */
	float m_deltat;
	unsigned int m_frame_number;
	float m_frame_deltat;
	double m_last_draw;
	
	// map related
	//@{
	// almost constant; cannot be changed if game has started
	int m_map_width, m_map_height;
	//@}
	
	// player related
	//@{
	void initPlayers();
	PlayerVector m_players;
	//@}
	
	bool m_is_game_ended;
};


} // end of namespace SC

#endif

