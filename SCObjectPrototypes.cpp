// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"

#include "smart_ptrs.h"
#include <string>
#include <list>
#include <map>

#include <cstdio>
#include <cstring>

#include "defs.h"
#include "luacpp/luacpp_lua.h"
#include "luacpp/luacpp.h"
#include "SCException.h"
#include "SCCoordinate.h"
#include "SCPlayer.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCObjectPrototypes.h"

using namespace SC;





ObjectPrototypes::ObjectPrototypes(Game *game)
	:m_game(game)
{
}

ObjectPrototypes::~ObjectPrototypes()
{
}


bool ObjectPrototypes::load(const char *listfile)
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
		int idx = luaL_checkint(L, -2);
		int objdata_idx = lua_gettop(L); // stack -1
		
		Object *obj = new Object(this->m_game);
		this->parseObjectData(obj, objdata_idx);
		
		ObjectSPtr_t objp(obj);
		this->m_obj_prototypes.addObject(objp);
		this->m_obj_protos_by_id[obj->getObjectId()] = objp;
		
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


void ObjectPrototypes::parseObjectData(Object *obj, int stack_idx)
{
	lua_State *L = this->m_lua.lua;
	
#define SET_OBJ_PROPERTY_INT(L_, tblidx_, name_) \
	do { \
		lua_pushliteral((L_), #name_); \
		lua_gettable((L_), (tblidx_)); \
		obj->m_##name_ = luaL_checkint((L_), -1); \
		lua_pop((L_), 1); \
	} while(0)
#define SET_OBJ_PROPERTY_NUMBER(L_, tblidx_, name_) \
	do { \
		lua_pushliteral((L_), #name_); \
		lua_gettable((L_), (tblidx_)); \
		obj->m_##name_ = luaL_checknumber((L_), -1); \
		lua_pop((L_), 1); \
	} while(0)
#define SET_OBJ_PROPERTY_STRING(L_, tblidx_, name_) \
	do { \
		lua_pushliteral((L_), #name_); \
		lua_gettable((L_), (tblidx_)); \
		obj->m_##name_ = luaL_checkstring((L_), -1); \
		lua_pop((L_), 1); \
	} while(0)
	
	SET_OBJ_PROPERTY_INT(L, stack_idx, object_type);
	SET_OBJ_PROPERTY_INT(L, stack_idx, object_id);
	SET_OBJ_PROPERTY_STRING(L, stack_idx, object_id_name);
	SET_OBJ_PROPERTY_STRING(L, stack_idx, object_name);
	SET_OBJ_PROPERTY_INT(L, stack_idx, race_id);
	
	{
		lua_pushliteral(L, "initial_state");
		lua_gettable(L, stack_idx);
		obj->m_initial_state = parseLuaObjectStateString(luaL_checkstring(L, -1));
		lua_pop(L, 1);
	}
	SET_OBJ_PROPERTY_INT(L, stack_idx, width);
	SET_OBJ_PROPERTY_INT(L, stack_idx, height);
	SET_OBJ_PROPERTY_INT(L, stack_idx, max_hp);
	SET_OBJ_PROPERTY_INT(L, stack_idx, max_energy);
	SET_OBJ_PROPERTY_INT(L, stack_idx, initial_minerals);
	SET_OBJ_PROPERTY_INT(L, stack_idx, initial_vespene_gas);
	SET_OBJ_PROPERTY_INT(L, stack_idx, provided_supplies);
	SET_OBJ_PROPERTY_INT(L, stack_idx, required_supplies);
	
	SET_OBJ_PROPERTY_NUMBER(L, stack_idx, armor);
	SET_OBJ_PROPERTY_NUMBER(L, stack_idx, damage);
	SET_OBJ_PROPERTY_NUMBER(L, stack_idx, moving_speed);
	SET_OBJ_PROPERTY_NUMBER(L, stack_idx, attack_speed);
	SET_OBJ_PROPERTY_NUMBER(L, stack_idx, attack_range);
	
	#undef SET_OBJ_PROPERTY_INT
	#undef SET_OBJ_PROPERTY_NUMBER
	#undef SET_OBJ_PROPERTY_STRING
}




