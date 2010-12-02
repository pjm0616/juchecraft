/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/ObjectFactory.h
**/

#ifndef SCObjectFactory_H_
#define SCObjectFactory_H_

namespace SC {


/** @brief Manages object prototypes
 */
class ObjectFactory
{
public:
	ObjectFactory(Game *game);
	~ObjectFactory();
	
	bool load(const char *listfile);
	
	/** finds object prototype by ObjectId
	 *  pointer returned by this functions become invalid when this object destructs.
	 *  @return constant referenct to the object prototype
	 */
	const ObjectPtr &findObjectById(ObjectId_t id) { return this->m_obj_protos_by_id[id]; }
	
	/** Creates a new object based on ObjectId `id'
	 *  @return a pointer to the newly created object
	 */
	ObjectPtr newObjectById(ObjectId_t id);
	
protected:
	ObjectList m_obj_prototypes; /**< List of object prototypes */
	ObjectTable m_obj_protos_by_id; /**< List of object prototypes, indexed by ObjectId */

private:
	/** @name internal function
	 *  @details parses data in `stack_idx' and stores the parsed information to `obj'
	 */
	void parseObjectData(Object *obj, int stack_idx);
	
	Game *m_game;
	luacpp m_lua; /**< a lua_State that holds object data */
};


}

#endif

