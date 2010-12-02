/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
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



} // end of namespace SC

#define END_OF_SCCoordinate_H_
#endif

