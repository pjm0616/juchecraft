/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/platform.h
**/


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

