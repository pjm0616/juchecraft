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
	
	void setUI(UserInterface *ui) { this->ui = ui; }
	
	void run();
	void test_tmp1(); void test_tmp2(); // 디버깅&테스트용
	
	Object *addObject(Object *obj) { return this->objects.addObject(obj); }
	int removeObject(Object *obj) { return this->objects.removeObject(obj); }
	
	double getElapsedTime() const { return this->elapsed_time; }
	double getFrameDelta() const { return this->frame_delta; }
	unsigned int getFrameNumber() const { return this->frame_number; }
	unsigned int getFPS() const { return 30; }
	float getCurrentFPS() const { return 1.0 / this->getFrameDelta(); }
	
	int getMapWidth() const { return this->map_width; }
	int getMapHeight() const { return this->map_height; }
	void getMapSize(int *width, int *height) { *width = this->getMapWidth(); *height = this->getMapHeight(); }
	// this function should be protected; map size will be set by calling loadMap()
	void setMapSize(int width, int height) { this->map_width = width; this->map_height = height; }
	
	// should be protected
	ObjectList &getObjectList() { return this->objects; }
	
protected:
	double getTime() const;
	
private:
	void processObjects();
	
	UserInterface *ui;
	ObjectList objects;
	
	time_t start_time;
	unsigned int frame_number;
	double frame_delta, elapsed_time;
	double last_draw;
	
	// almost constant; cannot be changed if game has started
	int map_width, map_height;
};


} // end of namespace SC

#endif

