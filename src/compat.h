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

/** @file src/compat.h
**/


#ifndef SC_COMPAT_H_
#define SC_COMPAT_H_

// FIXME
#ifdef __WIN32__
#ifndef srand48
# define srand48 srand
#endif
#ifndef lrand48
# define lrand48 rand
#endif
#ifndef usleep
# define usleep(x) Sleep((x) / 1000)
#endif
#ifndef getpid
# define getpid() 0
#endif
#endif

#endif

