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

/** @file src/game/Exception.cpp
**/

#include "config.h"

#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib>

#include "defs.h"
#include "game/Exception.h"

#ifdef DEBUG
void SC::trigger_assertion_failure(const char *file, int line, const char *func, const char *expr, const char *comment)
{
	std::stringstream buf;
	buf << "Assertion failed at " 
		<< func << "()"
		<< " ["
			<< file << ":" << line
		<< "] "
		<< "\"" << expr << "\"";
	if(comment && *comment)
		buf << " - \"" << comment << "\"";
	
	#ifdef DEBUG
	std::cerr << buf << std::endl;
	abort();
	#else
	throw new SC::Exception(buf.str());
	#endif
}
#endif



