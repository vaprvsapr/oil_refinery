#include "Structures.h"

#include <tuple>

Coordinate3D operator+(
	const Coordinate3D& lhs,
	const Coordinate3D& rhs
	)
{
	return
	{
		lhs.x + rhs.x,
		lhs.y + rhs.y,
		lhs.z + rhs.z
	};
}

bool operator==(const Coordinate3D& lhs, const Coordinate3D& rhs)
{
	return make_tuple(lhs.x, lhs.y, lhs.z) == make_tuple(rhs.x, rhs.y, rhs.z);
}