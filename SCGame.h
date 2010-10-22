// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

/** @file SCGame.h
**/


#ifndef SCGame_H_
#define SCGame_H_

namespace SC {


class UserInterface;
class Game
{
	friend class UserInterface;
	
public:
	Game();
	~Game();
	
	void setUI(UserInterface *ui) { this->m_ui = ui; }
	
	void run();
	void test_tmp1(); // 디버깅&테스트용
	
	// adds/removes an object.
	// DO NOT call this function while iterating ObjectList.
	Object *addObject(Object *obj) { return this->getObjectList().addObject(obj); }
	int removeObject(Object *obj) { return this->getObjectList().removeObject(obj); }
	
	double getElapsedTime() const;
	inline float getDelta() const { return this->m_deltat; }
	inline float getFrameDelta() const { return this->m_frame_deltat; }
	unsigned int getFrameNumber() const { return this->m_frame_number; }
	unsigned int getUpdateRate() const { return 60; } // limit to 60 updates per second
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
	
	// There's no point having an accessor for m_ui.
	//UserInterface *getUI() { return this->m_ui; }
	
	void endGame() { this->m_is_game_ended = true; }
	inline bool isGameEnded() const { return this->m_is_game_ended; }
	
protected:
	
private:
	void setStartTime(time_t t) { this->m_start_time = t; }
	inline time_t getStartTime() const { return this->m_start_time; }
	void setFrameNumber(unsigned int n) { this->m_frame_number = n; }
	inline void increaseFrameNumber(unsigned int n = 1) { this->m_frame_number += n; }
	inline void setDelta(float val) { this->m_deltat = val; }
	inline void setFrameDelta(float val) { this->m_frame_deltat = val; }
	void setLastDrawTime(double val) { this->m_last_draw = val; }
	inline double getLastDrawTime() const { return this->m_last_draw; }
	
	void processObjects();
	
	UserInterface *m_ui;
	ObjectList m_objects;
	
	time_t m_start_time;
	float m_deltat;
	unsigned int m_frame_number;
	float m_frame_deltat;
	double m_last_draw;
	
	// almost constant; cannot be changed if game has started
	int m_map_width, m_map_height;
	
	bool m_is_game_ended;
};


} // end of namespace SC

#endif

