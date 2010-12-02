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
	bool isClear() const { return (this->getType() == Type::None); }
	bool isCoordinateTarget() const { return (this->getType() == Type::Coordinate); }
	bool isObjectTarget() const { return (this->getType() == Type::Object); }
	
	void set(const Coordinate &coord);
	void set(const ObjectPtr &obj);
	const Coordinate &getCoordinate() const
	{
		SCAssert(this->getType() == Type::Coordinate);
		return this->m_target.coord;
	}
	const ObjectPtr &getObject() const
	{
		SCAssert(this->getType() == Type::Object);
		return this->m_target.obj;
	}
	const Coordinate &getAsCoordinate() const;
	
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

