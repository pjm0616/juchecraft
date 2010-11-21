// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"

#ifdef DEBUG
#include <stdio.h>
#endif

#include "smart_ptrs.h"
#include <string>
#include <cmath>
#include <cstdlib>
#include <cassert>

#include "defs.h"
#include "game/Exception.h"
#include "game/Coordinate.h"

using namespace SC;




static float rad2degf(float rad)
{
	return rad * 180 / M_PI;
}
static float deg2radf(float deg)
{
	return deg * M_PI / 180;
}

// dx, dy: coordinates in Cartesian coordinate system
// float angle = calculateAngle_Cartesian((x2 - x1), -(y2 - y1));
// float angle = calculateAngle_Cartesian(dx, -dy);
static float calculateAngle_Cartesian(float dx, float dy)
{
	float deg;
	if(dx == 0.0)
	{
		if(dy != 0.0)
			deg = (dy>0 ? 90.0 : 270.0);
		else
			deg = 0.0;
	}
	else
	{
		float rad = atan2f(dy, dx);
		deg = rad2degf(rad);
		
		// 0	~ 180	deg	=>	+0	~	+pi	rad
		// 180	~ 360	deg	=>	-pi	~	-0	rad
		if(deg < 0)
			deg = 360 + deg;
		
		//fprintf(stderr, "Angle: %f rad, %f deg; dx: %f, dy: %f, m: %f\n", rad, deg, dx, dy, dy/dx);
	}
	assert(deg >= 0 && deg <= 360);
	
	return deg;
}

float Coordinate::calculateAngle(const Coordinate &dest) const
{
	float x1 = this->getX(), y1 = this->getY();
	float x2 = dest.getX(), y2 = dest.getY();
	
	float angle = calculateAngle_Cartesian((x2 - x1), -(y2 - y1));
	return angle;
}

float Coordinate::calculateDistance(const Coordinate &dest) const
{
	float dx = dest.getX() - this->getX();
	float dy = dest.getY() - this->getY();
	
	return sqrt( dx*dx + dy*dy );
}



/* static */
void Coordinate::normalizeTopLeftCoordinate(Coordinate &top_left, Coordinate &bottom_right)
{
	int tl_x = top_left.getX(), br_x = bottom_right.getX();
	int tl_y = top_left.getY(), br_y = bottom_right.getY();
	
	if(tl_x > br_x)
	{
		top_left.setX(br_x);
		bottom_right.setX(tl_x);
	}
	if(tl_y > br_y)
	{
		top_left.setY(br_y);
		bottom_right.setY(tl_y);
	}
}






