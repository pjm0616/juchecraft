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

/** @file src/game/Target.h
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
	/** Get target as coordinate.
	 *  @return The coordiinate.
	 *  @retval If the target is an Object, returns current position of the object.
	 *  @retval If the target is an Coordinate, returns itself.
	 */
	const Coordinate &getAsCoordinate() const;
	
private:
	unsigned int m_type; /**< Type of the target */
	struct
	{
		Coordinate coord; /**< Coordinate target */
		ObjectPtr obj; /**< Object target */
	} m_target;
};



} // end of namespace SC

#define END_OF_SCTarget_H_
#endif

