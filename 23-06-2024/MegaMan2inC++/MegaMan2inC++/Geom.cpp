#include "Geom.h"
#include <cmath> // usa-se para calcular valor absoluto e raiz quadrada

Geom::Geom()
{
	x = y = 0.0f;
	Type = UNDEFINED_TYPE;
}

Geom::~Geom()
{

}



Point::Point()
{
	Type = POINT_TYPE;
}

Point::Point(float PosX, float PosY)
{
	GeomMoveTo(PosX, PosY);
	Type = POINT_TYPE;
}

Point::Point(int PosX, int PosY)
{
	GeomMoveTo(float(PosX), float(PosY));
	Type = POINT_TYPE;
}

float Point::Distance(const Point& p) const
{
	float deltaX = abs(p.GetX() - x); // valor absoluto para evitar números negativos
	float deltaY = abs(p.GetY() - y);

	return sqrt(deltaX*deltaX + deltaY*deltaY);
}


Line::Line()
{
	a = Point();
	b = Point();
	Type = LINE_TYPE;
}

Line::Line(float ax, float ay, float bx, float by)
{
	a = Point(ax, ay);
	b = Point(bx, by);
	Type = LINE_TYPE;
}

Line::Line(Point& pa, Point& pb)
{
	a = pa;
	b = pb;
	Type = LINE_TYPE;
}




Rect::Rect()
{
	left = right = 0.0f;
	top = bottom = 0.0f;
	Type = RECT_TYPE;
}

Rect::Rect(float xa, float ya, float xb, float yb)
{
	left = xa;
	top = ya;
	right = xb;
	bottom = yb;
	Type = RECT_TYPE;
}

Rect::Rect(Point& a, Point& b)
{
	left = a.GetX();
	top = a.GetY();
	right = b.GetX();
	bottom = b.GetY();
	Type = RECT_TYPE;
}



Circle::Circle()
{
	Radius = 0.0f;
	Type = CIRCLE_TYPE;
}

Circle::Circle(float R)
{
	x = R;
	y = R;
	Radius = R;
	Type = CIRCLE_TYPE;
}