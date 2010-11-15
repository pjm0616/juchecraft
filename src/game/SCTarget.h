// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

/** @file SCTarget.h
**/


#ifndef SCTarget_H_
#define SCTarget_H_

namespace SC {


class Object;
class Target
{
public:
	enum TargetType: unsigned int
	{
		Coordinate, 
		Object
	};
	
	Target(const Coordinate &coord);
	Target(const ObjectPtr &obj);
	~Target();
	
	
private:
	struct
	{
		Coordinate coord;
		ObjectPtr obj;
	} m_target;
};



} // end of namespace SC

#define END_OF_SCTarget_H_
#endif

