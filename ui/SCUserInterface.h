// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUserInterface_H_
#define SCUserInterface_H_

namespace SC {

class UserInterface
{
public:
	UserInterface(Game *game)
		:game(game)
	{
	}
	virtual ~UserInterface() {}
	
	virtual unsigned int getUIFPS() const { return game->getFPS(); }
	
	virtual void processFrame() = 0;
	virtual void draw() = 0;
	
protected:
	Game *game;
	
	virtual void drawObject(Object &obj) = 0;
	void drawObjects();
};


} // end of namespace SC

#endif

