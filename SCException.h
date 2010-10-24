// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

/** @file SCException.h
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


} // end of namespace SC

#endif

