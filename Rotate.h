#pragma once

#include <cmath>
#include "Structures.h"

Coordinate3D Rotate(
	Axis axis,
	Coordinate3D& coordinate,
	const double& angle
);

Coordinate2D RotateZ(
	Coordinate2D& coordinate,
	const double& angle
);