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

/** @file libs/luacpp/luacpp.h
**/

#ifndef SC_LUACPP_H_INCLUDED
#define SC_LUACPP_H_INCLUDED


struct lua_State;


#if 0
#define lua_pushstdstring(L_, str_) lua_pushlstring((L_), (str_).c_str(), (str_).size())
#define lua_pushstdstring2(L_, str_) \
	({ \
		const std::string &tmp_ = (str_); \
		lua_pushlstring((L_), tmp_.c_str(), tmp_.size()); \
	})
#define lua_pushstdstring_nl(L_, str_) lua_pushstring((L_), (str_).c_str())
#endif


#if 0
#define DEFINE_LUA_TOUSERDATA_FXN(_fxnname, _name, _type) \
	static _type *lua_to ##_fxnname(lua_State *L, int index) \
	{ \
		_type **ptr = (_type **)lua_touserdata(L, index); \
		if(!ptr) \
			luaL_typerror(L, index, #_name); \
		return *ptr; \
	}
#define DEFINE_LUA_CHECKUSERDATA_FXN(_fxnname, _name, _type) \
	static _type *lua_check ##_fxnname(lua_State *L, int index) \
	{ \
		luaL_checktype(L, index, LUA_TUSERDATA); \
		_type **ptr = (_type **)luaL_checkudata(L, index, #_name); \
		if(!ptr) \
			luaL_typerror(L, index, #_name); \
		if(!*ptr) \
			luaL_error(L, "null " #_name " object"); \
		return *ptr; \
	}
#define DEFINE_LUA_PUSHUSERDATA_FXN(_fxnname, _name, _type) \
	static _type **lua_push_ ##_fxnname(lua_State *L, _type *data) \
	{ \
		_type **ptr = (_type **)lua_newuserdata(L, sizeof(_type *)); \
		*ptr = data; \
		luaL_getmetatable(L, #_name); \
		lua_setmetatable(L, -2); \
		return ptr; \
	}
#endif



class luacpp
{
public:
	luacpp();
	~luacpp();
	
	/*
		luacpp::pcall
		* Return values: 
			0: success
			LUA_ERRRUN: a runtime error.
			LUA_ERRMEM: memory allocation error. For such errors, Lua does not call the error handler function.
			LUA_ERRERR: error while running the error handler function.
	*/
	int pcall(int nargs, int nresults);
	// pops and returns first string in lua stack.
	std::string popstring();
	
	int dofile(const char *file);
	int dofile(const std::string &file){return this->dofile(file.c_str());}
	
	int dostring(const char *code);
	int dostring(const std::string &code){return this->dostring(code.c_str());}
	
	#if 0
	int vpcallarg(int nresults, const char *argfmt, va_list &ap);
	// pcallarg("int,bool,string,null", 3, true, "asdf", NULL);
	int pcallarg(int nresults, const char *argfmt, ...);
	int callfxn(const char *name, int nresults, const char *argfmt, ...);
	#endif
	
	const std::string &get_lasterr_str() const {return this->lasterr_str;}
	void set_lasterr_str(const std::string &str) {this->lasterr_str = str;}
	
	
	
	
	
	lua_State *lua;
	
private:
	std::string lasterr_str;
	
};



struct luaref
{
	luaref();
	luaref(lua_State *L, int stack_n);
	luaref(const luaref &o);
	~luaref();
	
	int ref(lua_State *L, int stack_n);
	void unref();
	void pushref(lua_State *L) const; // for threads
	void pushref() const;
	void clearref();
	bool operator==(const luaref &o) const;
	
	lua_State *lua;
	int ref_id;
	
};





bool lua_my_tostdstring(lua_State *L, int index, std::string *dest);
std::string lua_my_tostdstring(lua_State *L, int index);
bool lua_my_toint(lua_State *L, int index, int *dest);
int lua_my_toint(lua_State *L, int index);
















#endif /* SC_LUACPP_H_INCLUDED */

