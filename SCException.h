// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCException_H_
#define SCException_H_

namespace SC {

class Exception
{
public:
	Exception() {}
	Exception(const std::string &msg) {this->errmsg = msg;}
	~Exception() {}
	
	const std::string &getErrorMsg() const { return this->errmsg; }
	
private:
	std::string errmsg;
};


} // end of namespace SC

#endif

