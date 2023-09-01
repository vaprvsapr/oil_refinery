#pragma once

#include "Structures.h"
#include "GetPixelCoordinate.h"
#include "Projection.h"
#include "Unigine.h"
#include "Orientation.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>


pair<vector<Unigine::Math::Vec3>, vector<Unigine::Math::quat>> CreateTackTrajectory(
	Coordinate2D initial_point,
	Size2D delta,
	Size2D overlap, // {0.2, 0.3}
	double pitch,
	double height,
	double horizontal_field_of_view,
	ImageSize image_size,
	double sea_level
);

pair<vector<Coordinate3D>, vector<Unigine::Math::quat>> CreateTrajectoryForGeoscan201(
	Coordinate2D initial_point,
	Size2D delta,
	Size2D overlap,
	double horizontal_field_of_view,
	ImageSize image_size,
	size_t max_frame_rate,
	double spatial_resolution
);


pair<vector<Unigine::Math::Vec3>, vector<Unigine::Math::quat>> GetTrajectoryFromArdupilotLog(string log, float vertical_overlap);

pair<vector<Unigine::Math::Vec3>, vector<Unigine::Math::quat>> GetTrajectoryFromArdupilotLogTrueOverlap(string log, float vertical_overlap, ImageSize image_size, float VFOV);