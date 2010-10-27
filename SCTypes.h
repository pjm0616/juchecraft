// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

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
		
		Moving = 64, /**< Object is moving */
		Attacking = 128, /**< Object is attacking @detail does not include pending attack */
	};
	
	// FIXME, move to elsewhere
	static struct
	{
		ObjectState_t state;
		const char *str;
	} gs_objectstate_stringified[] = {
		{ObjectState::None, "None"}, 
		{ObjectState::Burrowed, "Burrowed"}, 
		{ObjectState::InTransit, "InTransit"}, 
		{ObjectState::Cloaked, "Cloaked"}, 
		{ObjectState::Invincible, "Invincible"}, 
		{ObjectState::Hallucinated, "Hallucinated"}, 
		{ObjectState::Moving, "Moving"}, 
		{ObjectState::Attacking, "Attacking"}, 
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
typedef SC::shared_ptr<Player> PlayerSPtr_t;
typedef std::vector<PlayerSPtr_t> PlayerVector;


class Object;
typedef SC::shared_ptr<Object> ObjectSPtr_t;
class Game;
class ObjectPrototypes;

//typedef std::list<ObjectSPtr> ObjectList;
class ObjectList;
typedef std::map<ObjectId_t, ObjectSPtr_t> ObjectTable;



}

#endif

