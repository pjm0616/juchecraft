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

/** @file src/game/Coordinate.h
**/

#ifndef SCCoordinate_H_
#define SCCoordinate_H_

namespace SC {


class Coordinate
{
public:
	Coordinate(): m_x(0.0), m_y(0.0) {}
	Coordinate(float x, float y): m_x(x), m_y(y) {}
	Coordinate(const Coordinate &o): m_x(o.getX()), m_y(o.getY()) {}
	~Coordinate() {}
	
	float getX() const { return this->m_x; }
	float getY() const { return this->m_y; }
	void get(float *x, float *y) const { *x = this->getX(); *y = this->getY(); }
	void get(int *x, int *y) const { *x = (int)this->getX(); *y = (int)this->getY(); }
	
	void setX(float x) { this->m_x = x; }
	void setY(float y) { this->m_y = y; }
	void set(const Coordinate &o) { this->setX(o.getX()); this->setY(o.getY()); }
	void set(float x, float y) { this->setX(x); this->setY(y); }
	
	void addX(float x) { this->m_x += x; }
	void addY(float y) { this->m_y += y; }
	void add(const Coordinate &o) { this->addX(o.getX()); this->addY(o.getY()); }
	void add(float x, float y) { this->addX(x); this->addY(y); }
	
	void mulX(float xmul) { this->m_x *= xmul; }
	void mulY(float ymul) { this->m_y *= ymul; }
	void mul(float n) { this->mulX(n); this->mulY(n); }
	
	bool isZero() const { return (this->getX() == 0.0 && this->getY() == 0.0); }
	bool isIntegralZero() const { return ((int)this->getX() == 0 && (int)this->getY() == 0); }
	
	bool equals(const Coordinate &o) const { return ((int)o.getX() == (int)this->getX()) && ((int)o.getY() == (int)this->getY()); }
	bool operator==(const Coordinate &o) const { return this->equals(o); }
	
	Coordinate operator-() const
	{
		return Coordinate(-this->getX(), -this->getY());
	}
	Coordinate operator+(const Coordinate &o) const
	{
		return Coordinate(this->getX() + o.getX(), this->getY() + o.getY());
	}
	Coordinate operator-(const Coordinate &o) const
	{
		return *this + (-o);
	}
	Coordinate operator*(float n) const
	{
		return Coordinate(this->getX() * n, this->getY() * n);
	}
	Coordinate operator/(float n) const
	{
		return *this * (1/n);
	}
	
	/** @brief Calculates angle from `this' to `dest'
	 *  @param dest The destination
	 *  @return Angle in degrees
	 */
	float calculateAngle(const Coordinate &dest) const;
	/** @brief Calculates distance from `this' to `dest'
	 *  @param dest The destination
	 *  @return The distance
	 */
	float calculateDistance(const Coordinate &dest) const;
	
	/** Normalizes top-left and bottom-right coordinate.
	 *  @param[in,out] top_left
	 *  @param[in,out] bottom_right
	 */
	static void normalizeTopLeftCoordinate(Coordinate &top_left, Coordinate &bottom_right);
	
private:
	float m_x, m_y;
};

/** 2D vector
 *  FIXME: move to somewhere else
 */
class Vector2
{
public:
	Vector2()
		: m_x(0.0)
		, m_y(0.0)
	{
	}
	Vector2(float x, float y)
		: m_x(x)
		, m_y(y)
	{
	}
	Vector2(const Vector2 &rhs)
		: m_x(rhs.m_x)
		, m_y(rhs.m_y)
	{
	}
	
	void set(float x, float y) { this->m_x = x; this->m_y = y; }
	void setX(float x) { this->m_x = x; }
	void setY(float y) { this->m_y = y; }
	float getX() const { return this->m_x; }
	float getY() const { return this->m_y; }
	
	Vector2 add(const Vector2 &rhs) { return Vector2(this->m_x + rhs.m_x, this->m_y + rhs.m_y); }
	Vector2 add(float x, float y) { return Vector2(this->m_x + x, this->m_y + y); }
	Vector2 mul(const Vector2 &rhs) { return Vector2(this->m_x * rhs.m_x, this->m_y * rhs.m_y); }
	Vector2 mul(float x, float y) { return Vector2(this->m_x * x, this->m_y * y); }
	Vector2 div(const Vector2 &rhs) { return Vector2(this->m_x / rhs.m_x, this->m_y / rhs.m_y); }
	Vector2 div(float x, float y) { return Vector2(this->m_x / x, this->m_y / y); }
	
	Vector2 operator-() { return Vector2(-m_x, -m_y); }
	Vector2 operator+(const Vector2 &rhs) { return this->add(rhs); }
	Vector2 operator-(const Vector2 &rhs) { return this->add(rhs); }
	Vector2 operator*(const Vector2 &rhs) { return this->mul(rhs); }
	Vector2 operator/(const Vector2 &rhs) { return this->div(rhs); }
	
private:
	float m_x, m_y;
};


} // end of namespace SC

#define END_OF_SCCoordinate_H_
#endif

