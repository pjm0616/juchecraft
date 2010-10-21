// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCUserInterface_H_
#define SCUserInterface_H_

namespace SC {

class UserInterface
{
public:
	UserInterface(Game *game)
		:m_game(game)
	{
		this->setUIFPS(this->getGame()->getFPS());
	}
	virtual ~UserInterface() {}
	
	virtual bool initUI() = 0;
	virtual bool cleanupUI() = 0;
	
	unsigned int getUIFPS() const { return this->m_ui_fps; }
	
	virtual void processFrame() = 0;
	virtual void draw() = 0;
	
protected:
	Game *getGame() { return this->m_game; }
	const Game *getGame() const { return this->m_game; }
	void setUIFPS(unsigned int fps) { this->m_ui_fps = fps; }
	
	virtual void drawObject(Object &obj) = 0;
	void drawObjects();
	
	Game *m_game;
	unsigned m_ui_fps;
};


} // end of namespace SC

#endif

