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
#include "SCException.h"
#include "SCCoordinate.h"

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
		float m = dy / dx;
		float rad = atan(m);
		deg = fabs(rad2degf(rad));
		
		if(dx < 0 && dy >= 0)
			deg = 180 - deg;
		else if(dx < 0 && dy < 0)
			deg = 180 + deg;
		else if(dx >= 0 && dy < 0)
			deg = 360 - deg;
		
		//fprintf(stderr, "Angle: %f deg; dx: %f, dy: %f, m: %f\n", deg, dx, dy, m);
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










