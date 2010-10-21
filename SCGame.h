// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

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
	
	double getElapsedTime() const { return this->m_elapsed_time; }
	double getFrameDelta() const { return this->m_frame_delta; }
	unsigned int getFrameNumber() const { return this->m_frame_number; }
	unsigned int getFPS() const { return 30; }
	float getCurrentFPS() const { return 1.0 / this->getFrameDelta(); }
	
	int getMapWidth() const { return this->m_map_width; }
	int getMapHeight() const { return this->m_map_height; }
	void getMapSize(int *width, int *height) { *width = this->getMapWidth(); *height = this->getMapHeight(); }
	// this function should be protected; map size will be set by calling loadMap()
	void setMapSize(int width, int height) { this->m_map_width = width; this->m_map_height = height; }
	
	// should be protected
	ObjectList &getObjectList() { return this->m_objects; }
	const ObjectList &getObjectList() const { return this->m_objects; }
	
	UserInterface *getUI() { return this->m_ui; }
	
protected:
	double getTime() const;
	
private:
	void setStartTime(time_t t) { this->m_start_time = t; }
	time_t getStartTime() const { return this->m_start_time; }
	void setFrameNumber(unsigned int n) { this->m_frame_number = n; }
	void increaseFrameNumber(unsigned int n = 1) { this->m_frame_number += n; }
	void setFrameDelta(double val) { this->m_frame_delta = val; }
	void setElapsedTime(double val) { this->m_elapsed_time = val; }
	void increaseElapsedTime(double val) { this->m_elapsed_time += val; }
	void setLastDrawTime(double val) { this->m_last_draw = val; }
	double getLastDrawTime() const { return this->m_last_draw; }
	
	void processObjects();
	
	UserInterface *m_ui;
	ObjectList m_objects;
	
	time_t m_start_time;
	unsigned int m_frame_number;
	double m_frame_delta, m_elapsed_time;
	double m_last_draw;
	
	// almost constant; cannot be changed if game has started
	int m_map_width, m_map_height;
};


} // end of namespace SC

#endif

