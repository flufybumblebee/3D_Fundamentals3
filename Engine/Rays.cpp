#include "Rays.h"

Rays::Rays(const Vec2& POSITION, const size_t& NRAYS)
	:
	position(POSITION)
{
	const float INCREMENT = 360.0f / NRAYS;

	Vec2 direction;
	for (float i = 0.0f; i < 360.0f; i += INCREMENT)
	{
		direction = Vec2(UP * Mat3::RotationZ(i * (PI / 180)));
		rays.emplace_back(position, direction);
	}
}

Rays::Rays(const Vec2& POSITION, const std::vector<Ray>& RAYS)
	:
	position(POSITION),
	rays(RAYS)
{}

void Rays::SetPosition(const Vec2 & POSITION)
{
	position = POSITION;
	if (!rays.empty())
	{
		for (auto& r : rays)
		{
			r.SetPosition(position);
		}
	}
}

void Rays::Cast(const std::vector<LineSegment>& LINES)
{
	beams.clear();

	if (rays.size() > 0 && LINES.size() > 0)
	{
		for (size_t i = 0; i < rays.size(); i++)
		//for( auto r : rays )
		{
			float record = 100000.0f;
			Vec2 closest = { 0.0f,0.0f };

			for (size_t j = 0; j < LINES.size(); j++)
			//for( auto l : LINES )
			{

				Vec2 intersection = rays[i].Cast(LINES[j]);

				if (intersection.x > 0.0f && intersection.y > 0.0f)
				{
					//Vec2 intersection = r.Cast(l);
					float length = (position - intersection).Len();
					if (length > 0.0f && length < record)
					{
						record = length;
						closest = intersection;
					}
				}
			}

			if (closest.x > 0.0f && closest.y > 0.0f)
			{
				beams.emplace_back(closest);
			}
		}
	}
}

void Rays::Draw(Graphics& gfx)
{
	if (rays.size() > 0)
	{
		for (auto& r : rays)
		{
			r.Draw(gfx);
		}
	}

	if (beams.size() > 0)
	{
		Color color = { 155,255,255,255 };
		for (auto& b : beams)
		{
			gfx.DrawLineAlpha((int)position.x, (int)position.y, (int)b.x, (int)b.y,color);
		}
	}
}