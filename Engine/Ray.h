#pragma once

#include "Vec3.h"

class Ray
{
private:
	Vec3 position;
	Vec3 direction;
public:
	Ray() = default;
	Ray(const Vec3& position, const Vec3& direction);
	Vec3 GetPosition() const;
	Vec3 GetDirection() const;
	Vec3 PointAtParameter(float t) const;
};

