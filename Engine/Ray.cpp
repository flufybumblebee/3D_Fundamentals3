#include "Ray.h"

Ray::Ray(const Vec3& position, const Vec3& direction)
	:
	position(position),
	direction(direction)
{
}

Vec3 Ray::GetPosition() const
{
	return position;
}

Vec3 Ray::GetDirection() const
{
	return direction;
}

Vec3 Ray::PointAtParameter(float t) const
{
	return position + direction * t;
}
