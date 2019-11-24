#pragma once

#include "Ray.h"
#include "Mat.h"
#include "Vec2.h"

#include <vector>
#include <cmath>

class Rays
{
private:
	const Vec3 UP = { 0.0f,1.0f,1.0f };
	Vec2 position;
	std::vector<Ray> rays;
	std::vector<Vec2> beams;
public:
	Rays() = default;
	Rays(const Vec2& POSITION, const size_t& NRAYS);
	Rays(const Vec2& POSITION, const std::vector<Ray>& RAYS);
	void SetPosition(const Vec2& POSITION);
	void Cast(const std::vector<LineSegment>& LINES);
	void Draw(Graphics& gfx);
};
