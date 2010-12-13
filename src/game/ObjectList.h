/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *	
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *	
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/ObjectList.h
 *  Implements object list to be used in SC::Game
**/

#ifndef SCObjectList_H_
#define SCObjectList_H_

namespace SC {


/** Object manager class
 */
class ObjectList
{
public:
	typedef std::list<ObjectPtr> objlist_t;
	typedef objlist_t::iterator iterator;
	typedef objlist_t::const_iterator const_iterator;
	typedef objlist_t::reverse_iterator reverse_iterator;
	typedef objlist_t::const_reverse_iterator const_reverse_iterator;
	
	ObjectList()
		: m_iterator_invalidated(false)
	{
	}
	~ObjectList()
	{
	}
	
	const ObjectPtr &addObject(const ObjectPtr &obj);
	int removeObject(const ObjectPtr &obj);
	void erase(iterator it);
	
	void clear();
	size_t size() const { return this->getObjects().size(); }
	bool empty() const { return this->getObjects().empty(); }
	
	iterator find(const ObjectPtr &obj);
	const_iterator find(const ObjectPtr &obj) const;
	
	iterator begin() { return this->getObjects().begin(); }
	const_iterator begin() const { return this->getObjects().begin(); }
	iterator end() { return this->getObjects().end(); }
	const_iterator end() const { return this->getObjects().end(); }
	
	reverse_iterator rbegin() { return this->getObjects().rbegin(); }
	const_reverse_iterator rbegin() const { return this->getObjects().rbegin(); }
	reverse_iterator rend() { return this->getObjects().rend(); }
	const_reverse_iterator rend() const { return this->getObjects().rend(); }
	
	// returns true if iterator has been invalidated by adding/removing objects.
	bool isIteratorInvalidated() const { return this->m_iterator_invalidated; }
	void resetIteratorChecker() { this->m_iterator_invalidated = false; }
	
	/** checks if `obj' collides with objects in this list
	 */
	bool testCollision(const ObjectPtr &obj);
	/** checks if `obj' will collide with objs in this list within next frame
	 */
	bool detectCollision(const ObjectPtr &obj);
	
private:
	void setIteratorAsInvalidated() { this->m_iterator_invalidated = true; }
	objlist_t &getObjects() { return this->m_objects; }
	const objlist_t &getObjects() const { return this->m_objects; }
	
	objlist_t m_objects;
	
	bool m_iterator_invalidated;
};




} // end of namespace SC

#endif

