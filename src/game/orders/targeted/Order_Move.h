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

/** @file src/game/orders/targeted/Order_Move.h
**/

#ifndef SCUnitOrder_Move_H_
#define SCUnitOrder_Move_H_

namespace SC {
namespace UnitOrder {

class Move: public TargetedOrder
{
	typedef TargetedOrder super;
public:
	Move();
	Move(const Target &target);
	
	virtual bool initOrder(const ObjectPtr &obj);
	virtual ProcessResult_t process(float time);
	virtual OrderPtr clone(OrderPtr cloned_order = null_order);
	
private:
	/** @name state information
	 */
	//@{
	struct
	{
		unsigned int step;
	} m_state;
	//@}
};


} /* END OF namespace UnitOrder */
} /* END OF namespace SC */

#endif

