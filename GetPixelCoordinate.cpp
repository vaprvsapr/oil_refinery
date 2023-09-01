#include "GetPixelCoordinate.h"

#pragma once

#include "Structures.h"
#include "Rotate.h"

Coordinate3D GetPixelCoordinate(
	MyCamera camera,
	const Pixel& pixel)
{
	// ”дивительно, но работает исправно.
	Coordinate3D pixel_coordinate =
	{
		camera.focal_length,
		camera.matrix_size.width *
		int(camera.image_size.width / 2 - pixel.horizontal) /
		camera.image_size.width,
		camera.matrix_size.height *
		int(camera.image_size.height / 2 - pixel.vertical) /
		camera.image_size.height
	};

	pixel_coordinate = Rotate(Y, pixel_coordinate, camera.orientation.pitch);
	pixel_coordinate = Rotate(X, pixel_coordinate, camera.orientation.roll);
	pixel_coordinate = Rotate(Z, pixel_coordinate, camera.orientation.yaw);

	pixel_coordinate.x += camera.position.x;
	pixel_coordinate.y += camera.position.y;
	pixel_coordinate.z += camera.position.z;

	return pixel_coordinate;
}

// ¬озвращает координаты углов данного пиксел€
array<Coordinate3D, 4> GetPixelCorners(
	MyCamera camera,
	const Pixel& pixel
)
{
	double pixel_size = camera.matrix_size.height /
		camera.image_size.height;

	array<Coordinate3D, 4> pixel_corners =
	{
		// top left
		Coordinate3D
		{
			camera.focal_length,
			camera.matrix_size.width *
			int(camera.image_size.width / 2 - pixel.horizontal) /
			camera.image_size.width - pixel_size / 2,
			camera.matrix_size.height *
			int(camera.image_size.height / 2 - pixel.vertical) /
			camera.image_size.height + pixel_size / 2
		},
		// top right
		Coordinate3D
		{
			camera.focal_length,
			camera.matrix_size.width *
			int(camera.image_size.width / 2 - pixel.horizontal) /
			camera.image_size.width + pixel_size / 2,
			camera.matrix_size.height *
			int(camera.image_size.height / 2 - pixel.vertical) /
			camera.image_size.height + pixel_size / 2
		},
		// bottom_left
		Coordinate3D
		{
			camera.focal_length,
			camera.matrix_size.width *
			int(camera.image_size.width / 2 - pixel.horizontal) /
			camera.image_size.width - pixel_size / 2,
			camera.matrix_size.height *
			int(camera.image_size.height / 2 - pixel.vertical) /
			camera.image_size.height - pixel_size / 2
		},
		// bottom_right
		Coordinate3D
		{
			camera.focal_length,
			camera.matrix_size.width *
			int(camera.image_size.width / 2 - pixel.horizontal) /
			camera.image_size.width + pixel_size / 2,
			camera.matrix_size.height *
			int(camera.image_size.height / 2 - pixel.vertical) /
			camera.image_size.height - pixel_size / 2
		}
	};

	pixel_corners[0] = Rotate(Y, pixel_corners[0], camera.orientation.pitch);
	pixel_corners[1] = Rotate(Y, pixel_corners[1], camera.orientation.pitch);
	pixel_corners[2] = Rotate(Y, pixel_corners[2], camera.orientation.pitch);
	pixel_corners[3] = Rotate(Y, pixel_corners[3], camera.orientation.pitch);

	pixel_corners[0] = Rotate(Z, pixel_corners[0], camera.orientation.yaw);
	pixel_corners[1] = Rotate(Z, pixel_corners[1], camera.orientation.yaw);
	pixel_corners[2] = Rotate(Z, pixel_corners[2], camera.orientation.yaw);
	pixel_corners[3] = Rotate(Z, pixel_corners[3], camera.orientation.yaw);

	pixel_corners[0] = Rotate(X, pixel_corners[0], camera.orientation.roll);
	pixel_corners[1] = Rotate(X, pixel_corners[1], camera.orientation.roll);
	pixel_corners[2] = Rotate(X, pixel_corners[2], camera.orientation.roll);
	pixel_corners[3] = Rotate(X, pixel_corners[3], camera.orientation.roll);

	pixel_corners[0] = pixel_corners[0] + camera.position;
	pixel_corners[1] = pixel_corners[1] + camera.position;
	pixel_corners[2] = pixel_corners[2] + camera.position;
	pixel_corners[3] = pixel_corners[3] + camera.position;

	return pixel_corners;
}