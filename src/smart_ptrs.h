/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/smart_ptrs.h
**/


#ifndef SC_SMART_PTRS_H_
#define SC_SMART_PTRS_H_

#ifndef SC_PLATFORM_H_
# error include "platform.h" first.
#endif

// always use boost
#define SMART_PTR_USE_BOOST

#ifdef __WIN32__
# define SMART_PTR_USE_BOOST
#endif

#ifdef SMART_PTR_USE_BOOST
# include <boost/shared_ptr.hpp>
# include <boost/weak_ptr.hpp>
#else
//# include <memory>
# include <tr1/memory>
#endif


namespace SC
{

#ifdef SMART_PTR_USE_BOOST
using boost::shared_ptr;
using boost::weak_ptr;
#else
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
#endif



/** class T must be a WeakPtrOwner object.
 *  The hierarchy becomes somewhat awakard, but well.. damn
 */
template<class T> class WeakPtrOwner
{
public:
	WeakPtrOwner() {}
	WeakPtrOwner(const shared_ptr<T> &ptr)
	{
		assert(ptr.get() == this);
		this->m_thisptr = ptr;
	}
	~WeakPtrOwner() {}
	
	/** Creates and returns a new shared_ptr<T> object.
	 *  This function must be called only once, immediately after the construction.
	 */
	shared_ptr<T> makeThisPtr()
	{
		assert(this->m_thisptr.expired() == true);
		shared_ptr<T> ptr(static_cast<T *>(this));
		this->m_thisptr = ptr;
		return ptr;
	}
	shared_ptr<T> getPtr() const { return m_thisptr.lock(); }
	
private:
	weak_ptr<T> m_thisptr;
};




}


#endif

