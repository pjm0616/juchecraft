// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef SCCoordinate_H_
#define SCCoordinate_H_

namespace SC {


class Object;
class Coordinate
{
public:
	Coordinate(): x(0.0), y(0.0) {}
	Coordinate(float x, float y): x(x), y(y) {}
	Coordinate(const Coordinate &o): x(o.x), y(o.y) {}
	~Coordinate() {}
	
	float getX() const { return this->x; }
	float getY() const { return this->y; }
	void get(float *x, float *y) const { *x = this->getX(); *y = this->getY(); }
	void get(int *x, int *y) const { *x = this->getX(); *y = this->getY(); }
	
	void setX(float x) { this->x = x; }
	void setY(float y) { this->y = y; }
	void set(const Coordinate &o) { this->setX(o.getX()); this->setY(o.getY()); }
	void set(float x, float y) { this->setX(x); this->setY(y); }
	
	void addX(float x) { this->x += x; }
	void addY(float y) { this->y += y; }
	void add(const Coordinate &o) { this->addX(o.getX()); this->addY(o.getY()); }
	void add(float x, float y) { this->addX(x); this->addY(y); }
	
	bool isZero() const { return (this->x == 0.0 && this->y == 0.0); }
	bool isIntegralZero() const { return ((int)this->x == 0 && (int)this->y == 0); }
	
	bool equals(const Coordinate &o) const { return ((int)o.x == (int)this->x) && ((int)o.y == (int)this->y); }
	bool operator==(const Coordinate &o) const { return this->equals(o); }
	
	Coordinate operator+(const Coordinate &o)
	{
		Coordinate coord(*this);
		coord.addX(o.getX());
		coord.addY(o.getY());
		return coord;
	}
	
	float calculateAngle(const Coordinate &dest) const;
	float calculateDistance(const Coordinate &dest) const;
	
private:
	float x, y;
};



} // end of namespace SC

#define END_OF_SCCoordinate_H_
#endif

