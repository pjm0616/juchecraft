// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details


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
#else
//# include <memory>
# include <tr1/memory>
#endif


namespace SC
{

#ifdef SMART_PTR_USE_BOOST
using boost::shared_ptr;
#else
using std::tr1::shared_ptr;
#endif

}


#endif

