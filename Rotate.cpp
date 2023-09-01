#include "Rotate.h"

Coordinate3D Rotate(
	Axis axis,
	Coordinate3D& coordinate,
	const double& angle)
{
	switch (axis)
	{
	case X:
		return
		{
			coordinate.x,
			cos(angle) * coordinate.y -
			sin(angle) * coordinate.z,
			sin(angle) * coordinate.y +
			cos(angle) * coordinate.z
		};
		break;
	case Y:
		return
		{
			cos(angle) * coordinate.x +
			sin(angle) * coordinate.z,
			coordinate.y,
		   -sin(angle) * coordinate.x +
			cos(angle) * coordinate.z
		};
		break;
	case Z:
		return
		{
			cos(angle) * coordinate.x -
			sin(angle) * coordinate.y,
			sin(angle) * coordinate.x +
			cos(angle) * coordinate.y,
			coordinate.z
		};
		break;
	}
}

Coordinate2D RotateZ(
	Coordinate2D& coordinate,
	const double& angle)
{
	return
	{
		cos(-angle) * coordinate.x -
		sin(-angle) * coordinate.y,
		sin(-angle) * coordinate.x +
		cos(-angle) * coordinate.y
	};
}