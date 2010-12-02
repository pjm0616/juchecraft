/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/game/ObjectFilter.h
**/

#ifndef SC_ObjectFilter_H_
#define SC_ObjectFilter_H_



class ObjectFilter
{
public:
	enum
	{
		None = 0, 
		Selectable = 1, 
		
		All = 0xffffffff
	};
};


#endif

