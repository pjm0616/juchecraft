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
	
	// pointer returned by this functions become invalid when this object destructs.
	const ObjectPtr &findObjectById(ObjectId_t id) { return this->m_obj_protos_by_id[id]; }
	
	// returns a new object based on object `id'
	ObjectPtr newObjectById(ObjectId_t id);
	
protected:
	ObjectList m_obj_prototypes;
	ObjectTable m_obj_protos_by_id;

private:
	void parseObjectData(Object *obj, int stack_idx);
	
	Game *m_game;
	luacpp m_lua;
};


}

#endif

