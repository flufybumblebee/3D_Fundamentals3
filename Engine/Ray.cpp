#include "Ray.h"

Ray::Ray(const Vec2& POSITION, const Vec2& DIRECTION)
	:
	position(POSITION),
	direction(DIRECTION)
{}

void Ray::SetPosition(const Vec2 & POSITION)
{
	position = POSITION;
}

void Ray::SetDirection(const Vec2& DIRECTION)
{
	direction = DIRECTION;
}

Vec2 Ray::GetPosition() const
{
	return position;
}

Vec2 Ray::GetDirection() const
{
	return direction;
}

void Ray::LookAt(const Vec2& LOOK)
{
	direction = LOOK - position;
	direction.Normalize();
}

Vec2 Ray::Cast(const LineSegment& line)
{
	//	based on "line line intersection" from wikipedia
	
	const float X1 = line.GetStartX();
	const float Y1 = line.GetStartY();
	const float X2 = line.GetEndX();
	const float Y2 = line.GetEndY();

	const float X3 = position.x;
	const float Y3 = position.y;
	const float X4 = position.x + direction.x;
	const float Y4 = position.y + direction.y;

	const float DENOMINATOR = (X1 - X2) * (Y3 - Y4) - (Y1 - Y2) * (X3 - X4);

	if (DENOMINATOR == 0)
	{
		return { 0.0f,0.0f };
	}

	const float T = +((X1 - X3) * (Y3 - Y4) - (Y1 - Y3) * (X3 - X4)) / DENOMINATOR;
	const float U = -((X1 - X2) * (Y1 - Y3) - (Y1 - Y2) * (X1 - X3)) / DENOMINATOR;

	if (T > 0.0f && T < 1.0f && U > 0)
	{
		const Vec2 RESULT = { X1 + T * (X2 - X1), Y1 + T * (Y2 - Y1) };
		return RESULT;
	}
	else
	{
		return { 0.0f,0.0f };
	}
}

void Ray::Draw(Graphics& gfx)
{
	gfx.DrawLine(position, direction * 10.0f + position, Colors::White);
}
