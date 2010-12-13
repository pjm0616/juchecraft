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

/** @file tools/grp_conv/convert_grp.cpp
**/

#include "config.h"

#include "smart_ptrs.h"
#include <string>
#include <list>
#include <map>
#include <vector>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <inttypes.h>
#include <cassert>

namespace zlib
{
#include <zlib.h>
}

#include "defs.h"
#include "compat.h"
#include "luacpp/luacpp_lua.h"
#include "luacpp/luacpp.h"

#include "my_sdl.h"
#include "libmpqgrp/grp.h"
#include "libmpq/SFmpqapi/SFmpqapi.h"

#define JCIMG_SAVE_SUPPORT
#include "jcimg/jcimg.cpp"

using namespace SDL;


static inline int SDL_LockSurfaceIfNeeded(SDL_Surface *sf)
{
	if(SDL_MUSTLOCK(sf))
		return SDL_LockSurface(sf);
	else
		return 0;
}
static inline void SDL_UnlockSurfaceIfNeeded(SDL_Surface *sf)
{
	if(SDL_MUSTLOCK(sf))
		SDL_UnlockSurface(sf);
}

static inline Uint32 *SDL_GetPixelPtr32(SDL_Surface *sf, int x, int y)
{
	return (Uint32 *)sf->pixels + y*sf->pitch/4 + x;
}
static inline void SDL_SetPixel32(SDL_Surface *sf, int x, int y, Uint32 color)
{
	Uint32 *pixel = SDL_GetPixelPtr32(sf, x, y);
	*pixel = color;
}
static inline Uint32 SDL_MapRGBA32(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	return (a << 24) | (b << 16) | (g << 8) | (r);
}
static inline void SDL_GetRGBA32(Uint32 color, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a)
{
	*r = (color & 0x000000ff) >> 0;
	*g = (color & 0x0000ff00) >> 8;
	*b = (color & 0x00ff0000) >> 16;
	*a = (color & 0xff000000) >> 24;
}


static void grp_setpix(void *surface, int x, int y, Uint32 color)
{
	SDL_SetPixel32((SDL_Surface *)surface, x, y, (color | 0xff000000));
}
static grp_pixel_funcs g_grp_pixelfuncs={grp_setpix, NULL};

static void grp_setpix_wirefram_green(void *surface, int x, int y, Uint32 color)
{
	SDL_SetPixel32((SDL_Surface *)surface, x, y, 0xff00ff00);
}
static grp_pixel_funcs g_grp_pixelfuncs_wirefram_green={grp_setpix_wirefram_green, NULL};


SDL_Surface *render_grp_frame(grp_data_t *grpdata, grp_palette_t *pal, int framenum, unsigned int grpflags = 0)
{
	grp_header_t *grpinfo = get_grp_info(grpdata);
	//grp_frameheader_t *frame = grp_get_frame_info(grpdata, framenum);
	SDL_Surface *sf = SDL_CreateRGBSurface(SDL_SWSURFACE, grpinfo->max_width, grpinfo->max_height, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
	
	SDL_LockSurfaceIfNeeded(sf);
	draw_grp(sf, 0, 0, &g_grp_pixelfuncs, grpdata, pal, framenum, grpflags, 0);
	SDL_UnlockSurfaceIfNeeded(sf);
	
	return sf;
}

SDL_Surface *render_grp_frame_flipped(grp_data_t *grpdata, grp_palette_t *pal, int framenum, bool do_hflip = false, bool do_vflip = false, unsigned int grpflags = 0)
{
	grp_header_t *grpinfo = get_grp_info(grpdata);
	grp_frameheader_t *frame = grp_get_frame_info(grpdata, framenum);
	SDL_Surface *sf = render_grp_frame(grpdata, pal, framenum, grpflags);
	int w = grpinfo->max_width;
	int h = grpinfo->max_height;
	
	if(do_hflip)
	{
		for(int y = 0; y < h; y++)
		{
			Uint32 *line = SDL_GetPixelPtr32(sf, 0, y);
			for(int x = 0; x < w/2; x++)
			{
				Uint32 tmp = line[x];
				line[x] = line[w-1 - x];
				line[w-1 - x] = tmp;
			}
		}
	}
	if(do_vflip)
	{
		int ymod = sf->pitch/4;
		for(int x = 0; x < w; x++)
		{
			Uint32 *col = (Uint32 *)sf->pixels + x;
			// todo: optimize this
			for(int y = 0; y < h/2; y++)
			{
				Uint32 tmp = col[y*ymod];
				col[y*ymod] = col[(h-1 - y)*ymod];
				col[(h-1 - y)*ymod] = tmp;
			}
		}
	}
	
	return sf;
}

void replace_unit_colors(SDL_Surface *sf, Uint32 newcolor)
{
	//static const Uint32 orig_unit_colors[] = {0xde00de, 0x5b005b, 0xbd00bd, 0x9c009c, 0x7c007c, 0x190019, 0xff00ff, 0x3a003a};
	Uint8 nc_r = (newcolor & 0x0000ff) >> 0;
	Uint8 nc_g = (newcolor & 0x00ff00) >> 8;
	Uint8 nc_b = (newcolor & 0xff0000) >> 16;
	
	for(int y = 0; y < sf->h; y++)
	{
		Uint32 *line = SDL_GetPixelPtr32(sf, 0, y);
		for(int x = 0; x < sf->w; x++)
		{
			Uint8 r, g, b, a;
			SDL_GetRGBA32(line[x], &r, &g, &b, &a);
			if(r != 0 && g == 0 && r == b)
			{
				// 0xff:orig_r = player_[rgb]:rendered_[rgb]
				Uint8 r2 = r * nc_r / 0xff;
				Uint8 g2 = r * nc_g / 0xff;
				Uint8 b2 = r * nc_b / 0xff;
				line[x] = SDL_MapRGBA32(r2, g2, b2, a);
			}
		}
	}
}

#define SHADOW_MAGIC_COLOR 0xef00ef
#define SHADOW_MAGIC_COLOR_RGBA 0xffef00ef
#define SHADOW_BLACK_ALPHA 0xa0
void render_shadow_image(SDL_Surface *sf)
{
	for(int y = 0; y < sf->h; y++)
	{
		Uint32 *line = SDL_GetPixelPtr32(sf, 0, y);
		for(int x = 0; x < sf->w; x++)
		{
			if(line[x] == SHADOW_MAGIC_COLOR_RGBA)
				line[x] = 0x000000 | (SHADOW_BLACK_ALPHA << 24); // black with alpha SHADOW_BLACK_ALPHA
		}
	}
}

// this is VERY slow
SDL_Surface *render_grp_frame_to_surface(grp_data_t *grpdata, grp_palette_t *pal, int framenum, 
	bool do_hflip = false, bool do_vflip = false, 
	Uint32 new_unit_color = 0x00000000, unsigned int grpflags = 0, 
	int *new_left = NULL, int *new_top = NULL)
{
	grp_frameheader_t *frame = grp_get_frame_info(grpdata, framenum);
	
	SDL_Surface *rmodel = render_grp_frame_flipped(grpdata, pal, framenum, do_hflip, do_vflip, grpflags);
	
	int frame_left = frame->left, frame_top = frame->top;
	{
		/* correct x, y values */
		grp_header_t *grpinfo = get_grp_info(grpdata);
		if(do_hflip)
		{
			frame_left = (grpinfo->max_width-1) - (frame->left+frame->width);
			if(new_left)
				*new_left = frame_left;
		}
		if(do_vflip)
		{
			frame_top = (grpinfo->max_height-1) - (frame->top+frame->height);
			if(new_top)
				*new_top = frame_top;
		}
	}
	
	if(new_unit_color != 0x00000000)
		replace_unit_colors(rmodel, new_unit_color);
	else if(grpflags & SHADOW_COLOR)
		render_shadow_image(rmodel);
	
	return rmodel;
}




int lua_push_grppal(lua_State *L, grp_palette_t *v)
{
	grp_palette_t **p = (grp_palette_t **)lua_newuserdata(L, sizeof(grp_palette_t *));
	*p = v;
	luaL_getmetatable(L, "grppal");
	lua_setmetatable(L, -2);
	return 1;
}

grp_palette_t *lua_checkgrppal(lua_State *L, int idx)
{
	grp_palette_t **p = (grp_palette_t **)luaL_checkudata(L, idx, "grppal");
	return *p;
}

int lua_loadgrppal(lua_State *L)
{
	const char *path = luaL_checkstring(L, 1);
	grp_palette_t *p = load_palette(path);
	if(!p)
	{
		luaL_error(L, "failed to open file");
		return 0;
	}
	
	lua_push_grppal(L, p);
	return 1;
}

static const luaL_reg sg_grppal_methods[] = 
{
	{"load", lua_loadgrppal}, 
	
	{NULL, NULL}
};

static int lua_grppal_gc(lua_State *L) // destroy
{
	grp_palette_t *gp = lua_checkgrppal(L, 1);
	free(gp);
	
	return 0;
}

static const luaL_reg sg_grppal_meta_methods[] = 
{
	{"__gc", lua_grppal_gc}, 
	{NULL, NULL}
};

void register_luagrppal(lua_State *L)
{
	luaL_newmetatable(L, "grppal");
	luaL_register(L, 0, sg_grppal_meta_methods);
	int metatable = lua_gettop(L);
	
	luaL_register(L, "grppal", sg_grppal_methods);
	int methods = lua_gettop(L);
	
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, methods);
	lua_rawset(L, metatable);
	
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, methods);
	lua_rawset(L, metatable); // hide metatable: metatable.__metatable = methods
	
	lua_pop(L, 2);
}





int lua_push_grp(lua_State *L, grp_data_t *v)
{
	grp_data_t **p = (grp_data_t **)lua_newuserdata(L, sizeof(grp_data_t *));
	*p = v;
	luaL_getmetatable(L, "grp");
	lua_setmetatable(L, -2);
	return 1;
}

grp_data_t *lua_checkgrp(lua_State *L, int idx)
{
	grp_data_t **p = (grp_data_t **)luaL_checkudata(L, idx, "grp");
	return *p;
}




int lua_loadgrp(lua_State *L)
{
	const char *path = luaL_checkstring(L, 1);
	grp_data_t *p = load_grp(path);
	if(!p)
	{
		luaL_error(L, "failed to open file");
		return 0;
	}
	
	lua_push_grp(L, p);
	return 1;
}

int lua_grp_info(lua_State *L)
{
	grp_data_t *grp = lua_checkgrp(L, 1);
	grp_header_t *ghdr = get_grp_info(grp);
	
	lua_createtable(L, 0, 3);
	
	lua_pushliteral(L, "nframes");
	lua_pushinteger(L, ghdr->nframes);
	lua_rawset(L, -3);
	
	lua_pushliteral(L, "max_width");
	lua_pushinteger(L, ghdr->max_width);
	lua_rawset(L, -3);
	
	lua_pushliteral(L, "max_height");
	lua_pushinteger(L, ghdr->max_height);
	lua_rawset(L, -3);
	
	return 1;
}

int lua_grp_frame_info(lua_State *L)
{
	grp_data_t *grp = lua_checkgrp(L, 1);
	int framenum = luaL_checkint(L, 2);
	grp_frameheader_t *gfhdr = grp_get_frame_info(grp, framenum);
	
	lua_createtable(L, 0, 4);
	
	lua_pushliteral(L, "left");
	lua_pushinteger(L, gfhdr->left);
	lua_rawset(L, -3);
	
	lua_pushliteral(L, "top");
	lua_pushinteger(L, gfhdr->top);
	lua_rawset(L, -3);
	
	lua_pushliteral(L, "width");
	lua_pushinteger(L, gfhdr->width);
	lua_rawset(L, -3);
	
	lua_pushliteral(L, "height");
	lua_pushinteger(L, gfhdr->height);
	lua_rawset(L, -3);
	
	return 1;
}

int lua_grp_render(lua_State *L)
{
	grp_data_t *grp = lua_checkgrp(L, 1);
	grp_palette_t *pal = lua_checkgrppal(L, 2);
	int framenum = luaL_checkint(L, 3);
	int do_hflip = lua_toboolean(L, 4); // 0
	int do_vflip = lua_toboolean(L, 5); // 0
	Uint32 new_unit_color = luaL_optnumber(L, 6, 0x00000000);
	int draw_shadow = lua_toboolean(L, 7); // 0
	
	grp_header_t *ghdr = get_grp_info(grp);
	grp_frameheader_t *gfhdr = grp_get_frame_info(grp, framenum);
	
	unsigned int grpflags = 0;
	if(draw_shadow)
		grpflags |= (SHADOW_COLOR | (SHADOW_MAGIC_COLOR << 8));
	
	int new_left = gfhdr->left, new_top = gfhdr->top;
	SDL_Surface *sf = render_grp_frame_to_surface(grp, pal, framenum, do_hflip, do_vflip, new_unit_color, grpflags, 
		&new_left, &new_top);
	
	if(0)
	{
		SDL_Surface *scr = SDL_SetVideoMode(640,320, 32, SDL_SWSURFACE);
		SDL_BlitSurface(sf, NULL, scr, NULL);
		SDL_Flip(scr);
		sleep(5);
		SDL_Quit();
	}
	
	size_t size = (char *)(SDL_GetPixelPtr32(sf, sf->w-1, sf->h-1)+1) - (char *)sf->pixels;
	lua_pushlstring(L, (const char *)sf->pixels, size);
	
	{
		lua_createtable(L, 0, 4);
		
		lua_pushliteral(L, "left");
		lua_pushinteger(L, new_left);
		lua_rawset(L, -3);
	
		lua_pushliteral(L, "top");
		lua_pushinteger(L, new_top);
		lua_rawset(L, -3);
	
		lua_pushliteral(L, "width");
		lua_pushinteger(L, gfhdr->width);
		lua_rawset(L, -3);
	
		lua_pushliteral(L, "height");
		lua_pushinteger(L, gfhdr->height);
		lua_rawset(L, -3);
	}
	SDL_FreeSurface(sf);
	
	return 2;
}

static const luaL_reg sg_grp_methods[] = 
{
	{"load", lua_loadgrp}, 
	{"info", lua_grp_info}, 
	{"frameinfo", lua_grp_frame_info}, 
	{"render", lua_grp_render}, 
	
	{NULL, NULL}
};

static int lua_grp_gc(lua_State *L) // destroy
{
	grp_data_t *gd = lua_checkgrp(L, 1);
	free(gd);
	
	return 0;
}

static const luaL_reg sg_grp_meta_methods[] = 
{
	{"__gc", lua_grp_gc}, 
	{NULL, NULL}
};

void register_luagrp(lua_State *L)
{
	luaL_newmetatable(L, "grp");
	luaL_register(L, 0, sg_grp_meta_methods);
	int metatable = lua_gettop(L);
	
	luaL_register(L, "grp", sg_grp_methods);
	int methods = lua_gettop(L);
	
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, methods);
	lua_rawset(L, metatable);
	
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, methods);
	lua_rawset(L, metatable); // hide metatable: metatable.__metatable = methods
	
	lua_pop(L, 2);
}


////////////////////////// jcimage start




int lua_push_jcimage(lua_State *L, JucheImage *v)
{
	JucheImage **p = (JucheImage **)lua_newuserdata(L, sizeof(JucheImage *));
	*p = v;
	luaL_getmetatable(L, "jcimage");
	lua_setmetatable(L, -2);
	return 1;
}

JucheImage *lua_checkjcimage(lua_State *L, int idx)
{
	JucheImage **p = (JucheImage **)luaL_checkudata(L, idx, "jcimage");
	return *p;
}



void lua_parse_jcimg_info(lua_State *L, int infotbl_idx, jcimg_info_t *dest)
{
	dest->nimages = 0;
	
	lua_pushliteral(L, "max_width");
	lua_rawget(L, infotbl_idx);
	dest->max_width = luaL_checkint(L, -1);

	lua_pushliteral(L, "max_height");
	lua_rawget(L, infotbl_idx);
	dest->max_height = luaL_checkint(L, -1);
}

int lua_jcimage_new(lua_State *L)
{
	JucheImage *img = new JucheImage();
	
	if(lua_gettop(L) > 0)
	{
		jcimg_info_t info;
		lua_parse_jcimg_info(L, 1, &info);
		img->reset(&info);
	}
	
	lua_push_jcimage(L, img);
	
	return 1;
}

int lua_jcimage_reset(lua_State *L)
{
	JucheImage *img = lua_checkjcimage(L, 1);
	luaL_checktype(L, 2, LUA_TTABLE);
	int infotbl_idx = 2;
	
	jcimg_info_t info;
	lua_parse_jcimg_info(L, 1, &info);
	img->reset(&info);
	
	return 0;
}


int lua_jcimage_insertNewImage(lua_State *L)
{
	JucheImage *img = lua_checkjcimage(L, 1);
	luaL_checktype(L, 2, LUA_TTABLE);
	int infotbl_idx = 2;
	size_t size;
	const char *data = luaL_checklstring(L, 3, &size);
	
	jcimg_imginfo_t imginfo;
	{
		lua_pushliteral(L, "left");
		lua_rawget(L, infotbl_idx);
		imginfo.left = luaL_checkint(L, -1);
	
		lua_pushliteral(L, "top");
		lua_rawget(L, infotbl_idx);
		imginfo.top = luaL_checkint(L, -1);
	
		lua_pushliteral(L, "width");
		lua_rawget(L, infotbl_idx);
		imginfo.width = luaL_checkint(L, -1);
	
		lua_pushliteral(L, "height");
		lua_rawget(L, infotbl_idx);
		imginfo.height = luaL_checkint(L, -1);
	}
	
	img->insertNewImage(&imginfo, (uint32_t *)data, SDL_SWSURFACE);
	
	return 0;
}



int lua_jcimage_save(lua_State *L)
{
	JucheImage *img = lua_checkjcimage(L, 1);
	const char *filename = luaL_checkstring(L, 2);
	
	bool ret = img->save(filename);
	lua_pushinteger(L, ret);
	
	return 1;
}


static const luaL_reg sg_jcimage_methods[] = 
{
	{"new", lua_jcimage_new}, 
	{"reset", lua_jcimage_reset}, 
	{"insertNewImage", lua_jcimage_insertNewImage}, 
	{"save", lua_jcimage_save}, 
	
	{NULL, NULL}
};

static int lua_jcimage_gc(lua_State *L)
{
	JucheImage *img = lua_checkjcimage(L, 1);
	delete img;
	
	return 0;
}

static const luaL_reg sg_jcimage_meta_methods[] = 
{
	{"__gc", lua_jcimage_gc}, 
	{NULL, NULL}
};

void register_luajcimage(lua_State *L)
{
	luaL_newmetatable(L, "jcimage");
	luaL_register(L, 0, sg_jcimage_meta_methods);
	int metatable = lua_gettop(L);
	
	luaL_register(L, "jcimage", sg_jcimage_methods);
	int methods = lua_gettop(L);
	
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, methods);
	lua_rawset(L, metatable);
	
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, methods);
	lua_rawset(L, metatable); // hide metatable: metatable.__metatable = methods
	
	lua_pop(L, 2);
}


///// jcimage ends



int lua_z_compress(lua_State *L)
{
	size_t size;
	const char *srcdata = luaL_checklstring(L, 1, &size);
	
	size_t ressize = size+128;
	char *destbuf = new char[ressize];
	int ret = zlib::compress2((zlib::Bytef *)destbuf, (zlib::uLongf *)&ressize, (zlib::Bytef *)srcdata, size, 9);
	if(ret != Z_OK)
		return 0;
	
	lua_pushlstring(L, destbuf, ressize);
	delete[] destbuf;
	return 1;
}

int lua_z_uncompress(lua_State *L)
{
	size_t size;
	const char *srcdata = luaL_checklstring(L, 1, &size);
	
	size_t ressize = size * 8;
	char *destbuf = new char[ressize];
	int ret = zlib::uncompress((zlib::Bytef *)destbuf, (zlib::uLongf *)&ressize, (zlib::Bytef *)srcdata, size);
	if(ret != Z_OK)
		return 0;
	
	lua_pushlstring(L, destbuf, ressize);
	delete[] destbuf;
	return 1;
}

int lua_my_z_compress(lua_State *L)
{
	size_t size;
	const char *srcdata = luaL_checklstring(L, 1, &size);
	
	size_t result_size;
	char *buf = my_z_compress(srcdata, size, &result_size);
	
	lua_pushlstring(L, buf, result_size);
	delete[] buf;
	return 1;
}

int lua_my_z_uncompress(lua_State *L)
{
	size_t size;
	const char *srcdata = luaL_checklstring(L, 1, &size);
	
	size_t result_size;
	char *buf = my_z_uncompress(srcdata, size, &result_size);
	
	lua_pushlstring(L, buf, result_size);
	delete[] buf;
	return 1;
}

static SDL_PixelFormat sg_jcimg_pixelformat = {
	/*.palette =*/ NULL, 
	
	/*.BitsPerPixel =*/ 32, 
	/*.BytesPerPixel =*/ 4, 
	/*.Rloss =*/ 0, 
	/*.Gloss =*/ 0, 
	/*.Bloss =*/ 0, 
	/*.Aloss =*/ 0, 
	/*.Rshift =*/ 0, 
	/*.Gshift =*/ 8, 
	/*.Bshift =*/ 16, 
	/*.Ashift =*/ 24, 
	/*.Rmask =*/ 0x000000ff, 
	/*.Gmask =*/ 0x0000ff00, 
	/*.Bmask =*/ 0x00ff0000, 
	/*.Amask =*/ 0xff000000, 

	/*colorkey =*/ 0, 
	/*alpha =*/ 0xff, 
};

// checks if `a' and `b' are compatible format
static bool SDL_ComparePixelFormat(SDL_PixelFormat *a, SDL_PixelFormat *b)
{
	int ret = 0;
	
	#define COMPARE(name_) ret += !(a->name_ == b->name_)
	COMPARE(BitsPerPixel);
	COMPARE(BytesPerPixel);
	COMPARE(Rloss);
	COMPARE(Gloss);
	COMPARE(Bloss);
	COMPARE(Aloss);
	COMPARE(Rshift);
	COMPARE(Gshift);
	COMPARE(Bshift);
	COMPARE(Ashift);
	COMPARE(Rmask);
	COMPARE(Gmask);
	COMPARE(Bmask);
	COMPARE(Amask);
	#undef COMPARE
	
	return ret == 0;
}

int lua_load_image(lua_State *L)
{
	const char *filename = luaL_checkstring(L, 1);
	
	SDL_Surface *sf = IMG_Load(filename);
	if(!sf)
	{
		luaL_error(L, "failed to open file");
		return 0;
	}
	
	// we assume that IMG_Load gives surface in this format.
	if(!SDL_ComparePixelFormat(&sg_jcimg_pixelformat, sf->format))
	{
		SDL_Surface *sf2 = SDL_CreateRGBSurface(SDL_SWSURFACE, sf->w, sf->h, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
		SDL_BlitSurface(sf, NULL, sf2, NULL);
		SDL_FreeSurface(sf);
		sf = sf2;
	}
	
	size_t size = (char *)(SDL_GetPixelPtr32(sf, sf->w-1, sf->h-1)+1) - (char *)sf->pixels;
	lua_pushlstring(L, (const char *)sf->pixels, size);
	
	{
		lua_createtable(L, 0, 4);
		
		lua_pushliteral(L, "left");
		lua_pushinteger(L, 0);
		lua_rawset(L, -3);
	
		lua_pushliteral(L, "top");
		lua_pushinteger(L, 0);
		lua_rawset(L, -3);
	
		lua_pushliteral(L, "width");
		lua_pushinteger(L, sf->w);
		lua_rawset(L, -3);
	
		lua_pushliteral(L, "height");
		lua_pushinteger(L, sf->h);
		lua_rawset(L, -3);
	}
	SDL_FreeSurface(sf);
	
	return 2;
}




static HANDLE mpq_handles[3] = {(void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff};

static void init_mpq(const char *dir)
{
	char buf[1024];
	
	snprintf(buf, sizeof(buf), "%sStarDat.mpq", dir);
	SFileOpenArchive(buf, 1000, 0, &mpq_handles[0]);
	snprintf(buf, sizeof(buf), "%sBrooDat.mpq", dir);
	SFileOpenArchive(buf, 2000, 0, &mpq_handles[1]);
	snprintf(buf, sizeof(buf), "%spatch_rt.mpq", dir);
	SFileOpenArchive(buf, 3000, 0, &mpq_handles[2]);
	
	grp_set_file_method(GRP_USE_MPQ);
}



int lua_init_mpq(lua_State *L)
{
	const char *dir = luaL_checkstring(L, 1);
	init_mpq(dir);
	return 0;
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		puts("Usage: ./convert_grp <script_file>");
		return 1;
	}
	
	init_mpq("../../" GAME_DATA_DIR "./"); // hack, not needed, for compatibility
	
	luacpp Lc;
	lua_State *L = Lc.lua;
	
	register_luagrppal(L);
	register_luagrp(L);
	register_luajcimage(L);
	lua_register(L, "z_compress", lua_z_compress);
	lua_register(L, "z_uncompress", lua_z_uncompress);
	lua_register(L, "my_z_compress", lua_my_z_compress);
	lua_register(L, "my_z_uncompress", lua_my_z_uncompress);
	lua_register(L, "load_image", lua_load_image);
	lua_register(L, "init_mpq", lua_init_mpq);
	
	int ret = Lc.dofile(argv[1]);
	if(ret != 0)
	{
		puts(Lc.get_lasterr_str().c_str());
	}
	
	return 0;
}















