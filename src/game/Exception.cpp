/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
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



