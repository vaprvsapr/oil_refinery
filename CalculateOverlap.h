#pragma once
#include "Structures.h"
#include <iostream>


bool IsPointInsideATriangle(
	Coordinate2D pixel,
	array<Coordinate2D, 3>triangle)
{
	double a = (triangle[0].x - pixel.x) * (triangle[1].y - triangle[0].y)
		- (triangle[1].x - triangle[0].x) * (triangle[0].y - pixel.y);
	double b = (triangle[1].x - pixel.x) * (triangle[2].y - triangle[1].y)
		- (triangle[2].x - triangle[1].x) * (triangle[1].y - pixel.y);
	double c = (triangle[2].x - pixel.x) * (triangle[0].y - triangle[2].y)
		- (triangle[0].x - triangle[2].x) * (triangle[2].y - pixel.y);

	if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

#include "Structures.h"
#include <iostream>
#include "GetPixelCoordinate.h"
#include "Projection.h"
#include <math.h>
#include "fstream"

double CalculatePixelOverlap(const MyCamera& first, const MyCamera& second)
{
	Coordinate3D
		first_top_left_3d = GetPixelCoordinate(first,
			{ 0, 0 }),
		first_top_right_3d = GetPixelCoordinate(first,
			{ 0, first.image_size.width }),
		first_bottom_left_3d = GetPixelCoordinate(first,
			{ first.image_size.height, 0 }),
		first_bottom_right_3d = GetPixelCoordinate(first,
			{ first.image_size.height,
			first.image_size.width });

	Coordinate2D
		top_left = Projection(first.position, first_top_left_3d),
		top_right = Projection(first.position, first_top_right_3d),
		bottom_left = Projection(first.position, first_bottom_left_3d),
		bottom_right = Projection(first.position, first_bottom_right_3d);

	int pixel_count = 0;
	int total_pixel_count = 0;
	ofstream of;
	of.open("overlap.txt");
	for (unsigned int index_vertical = 0; index_vertical < second.image_size.height; index_vertical += 10)
	{
		for (unsigned int index_horizontal = 0; index_horizontal < second.image_size.width; index_horizontal += 10)
		{
			Coordinate3D pixel3d = GetPixelCoordinate(second, { index_vertical, index_horizontal });
			Coordinate2D pixel = Projection(second.position, pixel3d);

			bool b1 = IsPointInsideATriangle(pixel, { top_left, top_right, bottom_right });
			bool b2 = IsPointInsideATriangle(pixel, { top_left, bottom_left, bottom_right });

			if (b1 || b2)
			{
				pixel_count++;
				of << "0";
			}
			else
			{
				of << " ";
			}
			total_pixel_count++;
		}
		of << '\n';
	}
	return double(pixel_count) / total_pixel_count;
}

double CalculateAreaOverlap(const MyCamera& first, const MyCamera& second)
{
	Coordinate3D
		first_top_left_3d = GetPixelCoordinate(first,
			{ 0, 0 }),
		first_top_right_3d = GetPixelCoordinate(first,
			{ 0, first.image_size.width }),
		first_bottom_left_3d = GetPixelCoordinate(first,
			{ first.image_size.height, 0 }),
		first_bottom_right_3d = GetPixelCoordinate(first,
			{ first.image_size.height,
			first.image_size.width }),
		second_top_left_3d = GetPixelCoordinate(second,
			{ 0, 0 }),
		second_top_right_3d = GetPixelCoordinate(second,
			{ 0, second.image_size.width }),
		second_bottom_left_3d = GetPixelCoordinate(second,
			{ second.image_size.height, 0 }),
		second_bottom_right_3d = GetPixelCoordinate(second,
			{ second.image_size.height,
			second.image_size.width });

	Coordinate2D
		first_top_left = Projection(first.position, first_top_left_3d),
		first_top_right = Projection(first.position, first_top_right_3d),
		first_bottom_left = Projection(first.position, first_bottom_left_3d),
		first_bottom_right = Projection(first.position, first_bottom_right_3d),
		second_top_left = Projection(second.position, second_top_left_3d),
		second_top_right = Projection(second.position, second_top_right_3d),
		second_bottom_left = Projection(second.position, second_bottom_left_3d),
		second_bottom_right = Projection(second.position, second_bottom_right_3d);
	Coordinate2D
		rect_min = { min(second_top_left.x, min(second_top_right.x, min(second_bottom_left.x, second_bottom_right.x))),
		min(second_top_left.y, min(second_top_right.y, min(second_bottom_left.y, second_bottom_right.y))) },
		rect_max = { max(second_top_left.x, max(second_top_right.x, max(second_bottom_left.x, second_bottom_right.x))),
		max(second_top_left.y, max(second_top_right.y, max(second_bottom_left.y, second_bottom_right.y))) },
		rect_delta = { rect_max.x - rect_min.x, rect_max.y - rect_min.y };

	int d = 200;
	int pixel_count = 0, total_pixel_count = 0;
	for (double y = rect_min.y; y < rect_max.y; y += rect_delta.y / d)
	{
		for (double x = rect_min.x; x < rect_max.x; x += rect_delta.x / d)
		{
			bool a1 = IsPointInsideATriangle({ x, y }, { second_top_left, second_top_right, second_bottom_left });
			bool a2 = IsPointInsideATriangle({ x, y }, { second_top_right, second_bottom_left, second_bottom_right });
			bool b1 = IsPointInsideATriangle({ x, y }, { first_top_left, first_top_right, first_bottom_left });
			bool b2 = IsPointInsideATriangle({ x, y }, { first_top_right, first_bottom_left, first_bottom_right });

			if (a1 || a2)
				total_pixel_count++;
			if ((a1 || a2) && (b1 || b2))
				pixel_count++;
		}

	}
	return double(pixel_count) / total_pixel_count;
}

void CalculatePixelOverlapBetweenSetOfImages(const vector<MyCamera>& cameras)
{
	for (int i = 0; i < cameras.size(); i++)
	{
		for (int j = 0; j < cameras.size(); j++)
		{
			cout << CalculatePixelOverlap(cameras[i], cameras[j]) << " ";
		}
		cout << endl;
	}
}

void CalculateAreaOverlapBetweenSetOfImages(const vector<MyCamera>& cameras)
{
	for (int i = 0; i < cameras.size(); i++)
	{
		for (int j = 0; j < cameras.size(); j++)
		{
			cout << CalculateAreaOverlap(cameras[i], cameras[j]) << " ";
		}
		cout << endl;
	}
}
