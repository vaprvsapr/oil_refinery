#pragma once

#include <array>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

enum Axis
{
	X, Y, Z
};

enum Angle
{
	ROLL,
	PITCH,
	YAW
};

struct Coordinate3D
{
	double x;
	double y;
	double z;
};

Coordinate3D operator+(
	const Coordinate3D& lhs,
	const Coordinate3D& rhs
	);

bool operator==(const Coordinate3D& lhs, const Coordinate3D& rhs);

struct Coordinate2D
{
	double x;
	double y;
};

struct EulerAngles
{
	double roll;
	double pitch;
	double yaw;
};

struct ImageSize
{
	size_t height;
	size_t width;
};

struct Size2D
{
	double height;
	double width;
};

struct FieldOfView
{
	double vertical;
	double horizontal;
};

struct Pixel
{
	size_t vertical;
	size_t horizontal;
};

struct MyCamera
{

	Size2D matrix_size;
	ImageSize image_size;

	double vertical_field_of_view;
	double focal_length;

	Coordinate3D position;
	EulerAngles orientation;


	MyCamera(
		Coordinate3D _position,
		EulerAngles _orientation,
		double _vertical_field_of_view,
		ImageSize _image_size
	)
		: position(_position)
		, orientation(_orientation)
		, vertical_field_of_view(_vertical_field_of_view)
		, image_size(_image_size)
	{
		matrix_size = {
			double(image_size.height) / 1000,
			double(image_size.width) / 1000
		};
		focal_length =
			matrix_size.height / 2 /
			std::tan(vertical_field_of_view / 2);
	}
};

