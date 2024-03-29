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

/** @file src/game/Types.h
**/

#ifndef SCTypes_H_
#define SCTypes_H_

namespace SC {





/** @brief Defines race IDs.
 */
namespace RaceId
{
	typedef unsigned int RaceId_t;
	enum RaceId
	{
		None = 0, /**< Map objects, resources, bullets, etc. */
		Neutral = 1, /**< Neutral units */

		Terran = 2, 
		Protoss = 3, 
		Zerg = 4, 

		Juche = 5, 
		
		Size /**< Number of RaceId entries */
	};
}
using RaceId::RaceId_t;


/** @brief Defines object types
 */
namespace ObjectType
{
	typedef unsigned int ObjectType_t;
	
	enum ObjectType
	{
		None = 0, 
		Resource = 1, 
		Unit = 2, 
		Building = 3, 
		SIZE
	};
}
using ObjectType::ObjectType_t;


/** @brief Defines current object state
 */
namespace ObjectState
{
	typedef unsigned int ObjectState_t;
	
	enum ObjectState
	{
		None = 0, /**< In most cases, this is default */
		
		Burrowed = 1, /**< Unit is burrowed */
		InTransit = 2, /**< Building is in transit */
		Cloaked = 4, /**< Object is cloaked */
		Invincible = 8, /**< Object is invincible */
		Hallucinated = 16, /**< Object is hallucinated */
	};
	
	// FIXME, move to elsewhere
	static struct
	{
		ObjectState_t state;
		const char *str;
	} gs_objectstate_stringified[] ATTRIBUTE_UNUSED = {
		{ObjectState::None, "None"}, 
		{ObjectState::Burrowed, "Burrowed"}, 
		{ObjectState::InTransit, "InTransit"}, 
		{ObjectState::Cloaked, "Cloaked"}, 
		{ObjectState::Invincible, "Invincible"}, 
		{ObjectState::Hallucinated, "Hallucinated"}, 
		{ObjectState::None, NULL}
	};
}
using ObjectState::ObjectState_t;


/** @brief Defines the list of object IDs
 */
namespace ObjectId
{
	typedef unsigned int ObjectId_t;
	// enum ObjectId: see SCObjectIdList.h
}
using ObjectId::ObjectId_t;







typedef int PlayerId_t;
class Player;
typedef std::vector<Player *> PlayerVector;


class Object;
typedef SC::shared_ptr<Object> ObjectPtr;
typedef SC::weak_ptr<Object> ObjectWeakPtr;
class Game;
class ObjectFactory;

//typedef std::list<ObjectSPtr> ObjectList;
class ObjectList;
typedef std::map<ObjectId_t, ObjectPtr> ObjectTable;


namespace UnitAction {
	namespace ActionId
	{
		typedef unsigned int ActionId_t;
		// enum ActionId: see actions/ActionIdList.h
	}
	namespace ActionId // FIXME
	{
		enum ActionId
		{
			None, 
			Move, 
			Attack, 
			Produce, 
		};
	}
	
	using ActionId::ActionId_t;
	class Action;
	typedef SC::shared_ptr<Action> ActionPtr;
	typedef std::map<ActionId_t, ActionPtr> ActionTable;
}

namespace UnitOrder {
	namespace OrderId
	{
		typedef unsigned int OrderId_t;
		// enum OrderId: see Orders/OrderIdList.h
	}
	namespace OrderId
	{
		enum OrderId
		{
			None, 
			Move, 
			Attack, 
			Produce, 
		};
	}
	
	using OrderId::OrderId_t;
	class Order;
	typedef SC::shared_ptr<Order> OrderPtr;
	typedef std::map<OrderId_t, OrderPtr> OrderTable;
}

////// 

namespace SpecialAbilityId
{
	typedef unsigned int SpecialAbility_t;
}
using SpecialAbilityId::SpecialAbility_t;

namespace UnitOrderId
{
	typedef unsigned int UnitOrderId_t;
	// enum UnitOrderId: see SCUnitOrderIdList.h
}
using UnitOrderId::UnitOrderId_t;




class ObjectRenderingState;
typedef shared_ptr<ObjectRenderingState> ObjectRenderingStatePtr;

class ProductionInfo;
typedef shared_ptr<ProductionInfo> ProductionInfoPtr;


/** Defines requirements to do something
 */
struct OrderRequirements
{
public:
	OrderRequirements()
	{
	}
	
	bool checkRequirements(const ObjectPtr &obj)
	{
		return true;
	}
	
public:
	int hit_points;
	int shield;
	int energy;
	int minerals;
	int vespene_gas;
	int supplies;
	std::vector<ObjectId_t> objects;
	//std::vector<UpgradeId_t> upgrades;
};
typedef shared_ptr<OrderRequirements> OrderRequirementsPtr;
typedef shared_ptr<const OrderRequirements> OrderRequirementsConstPtr;









} /* END OF namespace SC */

#endif

