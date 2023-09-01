#include "Projection.h"

Coordinate2D Projection(
	const Coordinate3D& position,
	const Coordinate3D& pixel_coordinate)
{
	Coordinate3D pixel_coordinate_on_plane = { 0, 0, 0 };

	if (pixel_coordinate.x - position.x == 0)
		pixel_coordinate_on_plane.x = position.x;
	else
		pixel_coordinate_on_plane.x =
		(pixel_coordinate_on_plane.z - position.z) /
		(pixel_coordinate.z - position.z) *
		(pixel_coordinate.x - position.x) +
		position.x;

	if (pixel_coordinate.y - position.y == 0)
		pixel_coordinate_on_plane.y = position.y;
	else
		pixel_coordinate_on_plane.y =
		(pixel_coordinate_on_plane.z - position.z) /
		(pixel_coordinate.z - position.z) *
		(pixel_coordinate.y - position.y) +
		position.y;

	return {
		pixel_coordinate_on_plane.x,
		pixel_coordinate_on_plane.y
	};
}