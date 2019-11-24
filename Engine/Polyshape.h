#pragma once

#include "Graphics.h"
#include "Random.h"
#include "Vec2.h"

#include <vector>

class Polyshape
{
private:
	
	std::vector<Vec2> points;
public:
	Polyshape(const Polyshape& pts)
		:
		points(pts.points)
	{}
	Polyshape(Polyshape&& pts) noexcept
		:
		points(pts.points)
	{}
	Polyshape(const std::vector<Vec2>& points_in)
		:
		points(points_in)
	{}
	Polyshape(std::vector<Vec2>&& points_in)
		:
		points(points_in)
	{}
	/*Polyshape(const size_t NUMBER_OF_POINTS, const float MIN_X, const float MAX_X, const float MIN_Y, const float MAX_Y)
	{
		Vec2 random_point = { 0.0f,0.0f };

		for (size_t i = 0; i < NUMBER_OF_POINTS; i++)
		{
			random_point = Vec2(rnd::RandomFloat(MIN_X, MAX_X), rnd::RandomFloat(MIN_Y, MAX_Y));
			points.emplace_back(random_point);
		}
	}*/
	const std::vector<Vec2>& GetPoints() const
	{
		return points;
	}
	void Draw(Graphics& gfx)
	{
		for (size_t i = 0u; i < points.size() - 1u; i++)
		{
			gfx.DrawLine(points[i], points[i + 1u], Colors::White);
		}
		gfx.DrawLine(points.back(), points.front(), Colors::White);
	}
};
