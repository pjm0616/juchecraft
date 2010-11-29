
#include <string>
#include <sstream>

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
	throw new SC::Exception(buf.str());
}
#endif



