#ifndef SC_PLATFORM_H_
#define SC_PLATFORM_H_

#if !defined(SC_CONFIG_H_) || defined(END_OF_SC_CONFIG_H_)
# error Do not include this file directly. include "config.h" instead.
#endif


#if defined(WIN32) || defined(_WIN32)
# undef __WIN32__
# define __WIN32__ 1
#endif
//__CYGWIN__

#endif

