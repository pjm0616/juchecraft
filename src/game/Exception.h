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

