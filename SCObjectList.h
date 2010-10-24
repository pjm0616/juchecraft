// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

/** @file SCObjectList.h
 *  @brief Implements object list to be used in SC::Game
**/


#ifndef SCObjectList_H_
#define SCObjectList_H_

namespace SC {




//typedef std::list<ObjectSPtr> ObjectList;
typedef std::map<ObjectId_t, ObjectSPtr_t> ObjectMap;

class ObjectList
{
public:
	typedef std::list<ObjectSPtr_t> objlist_t;
	typedef objlist_t::iterator iterator;
	typedef objlist_t::const_iterator const_iterator;
	typedef objlist_t::reverse_iterator reverse_iterator;
	typedef objlist_t::const_reverse_iterator const_reverse_iterator;
	
	ObjectList()
		:m_iterator_invalidated(false)
	{
	}
	~ObjectList()
	{
	}
	
	const ObjectSPtr_t &addObject(const ObjectSPtr_t &obj);
	int removeObject(const ObjectSPtr_t &obj);
	void erase(iterator it);
	
	void clear() { this->getObjects().clear(); }
	size_t size() const { return this->getObjects().size(); }
	bool empty() const { return this->getObjects().empty(); }
	
	iterator find(const ObjectSPtr_t &obj);
	const_iterator find(const ObjectSPtr_t &obj) const;
	
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
	
private:
	void setIteratorAsInvalidated() { this->m_iterator_invalidated = true; }
	objlist_t &getObjects() { return this->m_objects; }
	const objlist_t &getObjects() const { return this->m_objects; }
	
	objlist_t m_objects;
	
	bool m_iterator_invalidated;
};




} // end of namespace SC

#endif

