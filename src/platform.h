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

