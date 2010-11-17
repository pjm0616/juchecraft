// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"


#include "smart_ptrs.h"
#include <string>
#include <list>
#include <map>
#include <vector>
#include <cstdio>
#include <cstring>
#include <deque>

#include "defs.h"
#include "luacpp/luacpp_lua.h"
#include "luacpp/luacpp.h"
#include "game/Types.h"
#include "game/Exception.h"
#include "game/Coordinate.h"
#include "game/ObjectIdList.h"
#include "actions/UnitAction.h"
#include "orders/UnitOrder.h"
#include "game/Object.h"
#include "game/ObjectList.h"
#include "game/ObjectFactory.h"
#include "game/Player.h"

using namespace SC;





ObjectFactory::ObjectFactory(Game *game)
	: m_game(game)
{
}

ObjectFactory::~ObjectFactory()
{
}


bool ObjectFactory::load(const char *listfile)
{
	lua_State *L = this->m_lua.lua;
	
	int ret = luaL_loadfile(L, listfile);
	if(ret != 0)
		throw new Exception("failed to load data: " + this->m_lua.popstring());
	ret = this->m_lua.pcall(0, 1);
	if(ret != 0)
		throw new Exception("failed to load data: " + this->m_lua.popstring());
	
	this->m_obj_protos_by_id.clear();
	this->m_obj_prototypes.clear();
	
	// lua stack: -1(data)
	int obj_data = lua_gettop(L);
	lua_pushnil(L);
	while(lua_next(L, obj_data) != 0)
	{
		// stack: -2(key), -1(value)
		//int idx = luaL_checkint(L, -2);
		int objdata_idx = lua_gettop(L); // stack -1
		
		Object *obj = new Object(this->m_game);
		this->parseObjectData(obj, objdata_idx);
		
		ObjectPtr ptr = obj->makeThisPtr();
		this->m_obj_prototypes.addObject(ptr);
		this->m_obj_protos_by_id[obj->getObjectId()] = ptr;
		
		lua_pop(L, 1);
	}
	
	return true;
}

static ObjectState_t convertStringToObjectState(const char *str)
{
	for(int i = 0; ; i++)
	{
		if(!ObjectState::gs_objectstate_stringified[i].str)
			break;
		if(strcmp(ObjectState::gs_objectstate_stringified[i].str, str) == 0)
			return ObjectState::gs_objectstate_stringified[i].state;
	}
	return ObjectState::None;
}
static ObjectState_t parseLuaObjectStateString(const char *str)
{
	return convertStringToObjectState(str);
}


void ObjectFactory::parseObjectData(Object *obj, int stack_idx)
{
	lua_State *L = this->m_lua.lua;
	
	// used vars: `L', `stack_idx', `obj'
#define SET_OBJ_PROPERTY_EXPR(name_, expr_) \
	do { \
		lua_pushliteral(L, #name_); \
		lua_gettable(L, stack_idx); \
		obj->m_constattrs.name_ = (expr_); \
		lua_pop(L, 1); \
	} while(0)
#define SET_OBJ_PROPERTY(type_, name_) SET_OBJ_PROPERTY_EXPR(name_, luaL_check##type_(L, -1))
	
	SET_OBJ_PROPERTY(int, object_type);
	SET_OBJ_PROPERTY(int, object_id);
	SET_OBJ_PROPERTY(string, object_id_name);
	SET_OBJ_PROPERTY(string, object_name);
	SET_OBJ_PROPERTY(int, race_id);
	
	SET_OBJ_PROPERTY_EXPR(initial_state, parseLuaObjectStateString(luaL_checkstring(L, -1)));
	
	SET_OBJ_PROPERTY(int, width);
	SET_OBJ_PROPERTY(int, height);
	SET_OBJ_PROPERTY(int, max_hp);
	SET_OBJ_PROPERTY(int, max_energy);
	SET_OBJ_PROPERTY(int, initial_minerals);
	SET_OBJ_PROPERTY(int, initial_vespene_gas);
	SET_OBJ_PROPERTY(int, provided_supplies);
	SET_OBJ_PROPERTY(int, required_supplies);
	
	SET_OBJ_PROPERTY(number, armor);
	SET_OBJ_PROPERTY(number, damage);
	SET_OBJ_PROPERTY(number, moving_speed);
	SET_OBJ_PROPERTY(number, attack_speed);
	SET_OBJ_PROPERTY(number, attack_range);
	
	#undef SET_OBJ_PROPERTY
	#undef SET_OBJ_PROPERTY_EXPR
}



ObjectPtr ObjectFactory::newObjectById(ObjectId_t id)
{
	const ObjectPtr &proto = this->findObjectById(id);
	if(!proto)
		throw new Exception("Cannot find object prototype");
	// return shared_ptr of newly created(cloned) object
	return proto->clone()->getPtr();
}

















