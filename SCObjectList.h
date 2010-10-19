// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCObjectList_H_
#define SCObjectList_H_

namespace SC {

class Game;
class ObjectList
{
public:
	typedef std::list<std::tr1::shared_ptr<Object> >::iterator iterator;
	typedef std::list<std::tr1::shared_ptr<Object> >::const_iterator const_iterator;
	typedef std::list<std::tr1::shared_ptr<Object> >::reverse_iterator reverse_iterator;
	typedef std::list<std::tr1::shared_ptr<Object> >::const_reverse_iterator const_reverse_iterator;
	
	ObjectList(Game &game) :game(game) {}
	~ObjectList()
	{
		this->removeAllObjects();
	}
	
	Object *addObject(Object *obj);
	int removeObject(Object *obj);
	void removeAllObjects();
	
	iterator begin() { return this->objects.begin(); }
	const_iterator begin() const { return this->objects.begin(); }
	iterator end() { return this->objects.end(); }
	const_iterator end() const { return this->objects.end(); }
	
	reverse_iterator rbegin() { return this->objects.rbegin(); }
	const_reverse_iterator rbegin() const { return this->objects.rbegin(); }
	reverse_iterator rend() { return this->objects.rend(); }
	const_reverse_iterator rend() const { return this->objects.rend(); }
	
private:
	Game &game;
	std::list<std::tr1::shared_ptr<Object> > objects;
};



} // end of namespace SC

#endif

