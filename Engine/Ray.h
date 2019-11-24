#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "LineSegment.h"

class Ray
{
private:
	Vec2 position = { 0.0f, 0.0f };
	Vec2 direction = { 0.0f, 0.0f };
public:
	Ray() = default;
	Ray(const Vec2& POSITION, const Vec2& DIRECTION);
	void SetPosition(const Vec2& POSITION);	
	void SetDirection(const Vec2& DIRECTION);
	Vec2 GetPosition() const;
	Vec2 GetDirection() const;
	void LookAt(const Vec2& look);
	Vec2 Cast(const LineSegment& line);
	void Draw(Graphics& gfx);
};

