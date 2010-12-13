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

/** @file src/game/Exception.h
**/

#ifndef SCException_H_
#define SCException_H_

namespace SC {

class Exception
{
public:
	Exception() {}
	Exception(const std::string &msg) {this->m_errmsg = msg;}
	Exception(const char *msg) {this->m_errmsg = msg;}
	~Exception() {}
	
	const std::string &getErrorMsg() const { return this->m_errmsg; }
	
private:
	std::string m_errmsg;
};

#ifdef DEBUG
void trigger_assertion_failure(const char *file, int line, const char *func, const char *expr, const char *comment);
#define SCAssert(expr_) \
	do { \
		if(unlikely(!(expr_))) { \
			/*trigger_assertion_failure(__FILE__, __LINE__, __func__, #expr_, NULL);*/ \
			trigger_assertion_failure(__FILE__, __LINE__, __FUNCTION__, #expr_, NULL); \
		} \
	} while(0)
#else
#define SCAssert(expr_) do{}while(0)
#endif


} // end of namespace SC

#endif

