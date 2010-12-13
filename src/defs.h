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

/** @file src/defs.h
**/


#ifndef SC_DEFS_H_
#define SC_DEFS_H_


// for msvc
#ifdef _DEBUG
# define DEBUG
#endif

#if !defined(NDEBUG) && !defined(DEBUG)
# error Neither `NDEBUG' nor `DEBUG' is defined.
#endif
#if defined(NDEBUG) && defined(DEBUG)
# error Both `NDEBUG' and `DEBUG' are defined.
#endif


#ifdef __GNUC__
/* `!!' resolves some issues related to boolean expression overloading */
# define likely(expr_) __builtin_expect(!!(expr_), 1)
# define unlikely(expr_) __builtin_expect(!!(expr_), 0)
# define prefetch_read(addr_) __builtin_prefetch((addr_), 0)
# define prefetch_write(addr_) __builtin_prefetch((addr_), 1)
# define ATTRIBUTE_UNUSED __attribute__((unused))
# define ATTRIBUTE_ALWAYS_INLINE __attribute__((always_inline))
#else
# define likely(expr_) (!!(expr_))
# define unlikely(expr_) (!!(expr_))
# define prefetch_read(addr_) do{}while(0)
# define prefetch_write(addr_) do{}while(0)
# define ATTRIBUTE_UNUSED
# define ATTRIBUTE_ALWAYS_INLINE
#endif

#ifdef DEBUG
# define dynastatic_cast dynamic_cast
#else
# define dynastatic_cast static_cast
#endif

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

//#define _(str_) gettext(str_)
#define _(str_) (str_)

#ifdef _MSC_VER
# pragma warning(disable: 4482)
# pragma warning(disable: 4355 4819)
#endif

#ifdef __WIN32__
# define snprintf _snprintf
#endif

#ifdef __WIN32__
#define GAME_ROOT_DIR "../../"
#define GAME_DATA_DIR "../../../mini_sc_data/"
#else
#define GAME_ROOT_DIR "./"
#define GAME_DATA_DIR "../mini_sc_data/"
#endif

#endif

