// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCObjectList_H_
#define SCObjectList_H_

namespace SC {

class ObjectList
{
public:
	typedef std::list<std::tr1::shared_ptr<Object> > objlist_t;
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
		this->removeAllObjects();
	}
	
	Object *addObject(Object *obj);
	int removeObject(Object *obj);
	void removeAllObjects();
	
	iterator find(Object *obj);
	const_iterator find(Object *obj) const;
	
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

