#pragma once

#include "Structures.h"
#include "Rotate.h"

Coordinate3D GetPixelCoordinate(
	MyCamera camera,
	const Pixel& pixel
);

array<Coordinate3D, 4> GetPixelCorners(
	MyCamera camera,
	const Pixel& pixel
);