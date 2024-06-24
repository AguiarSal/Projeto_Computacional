#ifndef GEOM_H
#define GEOM_H

#include <list>
using std::list;

enum GeomTypes{UNDEFINED_TYPE, POINT_TYPE, LINE_TYPE, RECT_TYPE, CIRCLE_TYPE};

// classe mãe das formas geométricas
class Geom
{
protected:
	float x, y;
	unsigned int Type;

public:
	Geom();
	virtual ~Geom();


	virtual float GetX() const {
		return x;
	}
	virtual float GetY() const {
		return y;
	}
	virtual unsigned int GetType() const {
		return Type;
	}
	virtual void GeomTranslate(float DeltaX, float DeltaY) {
		x += DeltaX;
		y += DeltaY;
	}
	virtual void GeomMoveTo(float PosX, float PosY) {
		x = PosX;
		y = PosY;
	}

};


class Point : public Geom
{
public:
	Point();
	Point(float PosX, float PosY);
	Point(int PosX, int PosY);

	float Distance(const Point& p) const;

};

class Line : public Geom
{
public:
	Point a, b;

	Line();
	Line(float xa, float ya, float xb, float yb);
	Line(Point& pa, Point& pb);

	float Ax() const {
		return x + a.GetX();
	}
	float Ay() const {
		return y + a.GetY();
	}
	float Bx() const {
		return x + b.GetX();
	}
	float By() const {
		return y + b.GetY();
	}

};

class Rect : public Geom
{
public:
	float left;
	float top;
	float right;
	float bottom;

	Rect();
	Rect(float xa, float ya, float xb, float yb);
	Rect(Point& a, Point& b);

	float Left() const {
		return x + left;
	}
	float Top() const {
		return y + top;
	}
	float Right() const {
		return x + right;
	}
	float Bottom() const {
		return y + bottom;
	}

};

class Circle : public Geom
{
public:
	float Radius;

	Circle();
	Circle(float r);

	float GetCenterX() const {
		return x + Radius;
	}
	float GetCenterY() const {
		return y + Radius;
	}
};

#endif