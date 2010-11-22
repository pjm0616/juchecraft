// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

/** @file SCTarget.h
**/


#ifndef SCTarget_H_
#define SCTarget_H_

namespace SC {


class Target
{
public:
	struct Type
	{
		enum
		{
			None, 
			Coordinate, 
			Object
		};
	};
	
	Target();
	Target(const Coordinate &coord);
	Target(const ObjectPtr &obj);
	Target(const Target &o);
	~Target();
	
	void clear();
	int getType() const { return this->m_type; }
	
	void set(const Coordinate &coord);
	void set(const ObjectPtr &obj);
	const Coordinate &getCoordinate() const;
	const ObjectPtr &getObject() const;
	
private:
	unsigned int m_type;
	struct
	{
		Coordinate coord;
		ObjectPtr obj;
	} m_target;
};



} // end of namespace SC

#define END_OF_SCTarget_H_
#endif

