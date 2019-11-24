#pragma once

#include "Graphics.h"
#include "Vec2.h"

class LineSegment
{
private:
	Vec2 p0;
	Vec2 p1;
public:
	LineSegment(const float& x0, const float& y0, const float& x1, const float& y1)
		:
		p0(x0,y0),
		p1(x1,y1)
	{}
	float GetStartX() const
	{
		return p0.x;
	}
	float GetStartY() const
	{
		return p0.y;
	}
	float GetEndX() const
	{
		return p1.x;
	}
	float GetEndY() const
	{
		return p1.y;
	}
	void Draw(Graphics& gfx)
	{
		gfx.DrawLine(p0, p1, Colors::White);
	}
};
