#pragma once

#include <random>
#include <vector>
#include "Structures.h"
#include "Unigine.h"
#include <iostream>

vector<Unigine::Math::Vec3> GetNoisyCoordinates(
	const vector<Unigine::Math::Vec3>& coordinates,
	const array<double, 3> mean,
	const array<double, 3> sigma)
{
	random_device rd{};
	mt19937 gen{ rd() };
	normal_distribution<float>
		dx(mean[0], sigma[0]),
		dy(mean[1], sigma[1]),
		dz(mean[2], sigma[2]);

	vector<Unigine::Math::Vec3> noisy_coordinates;

	for (size_t i = 0; i < coordinates.size(); i++)
	{
		noisy_coordinates.push_back({
			coordinates[i].x + dx(gen),
			coordinates[i].y + dy(gen),
			coordinates[i].z + dz(gen)
			});
	}
	return noisy_coordinates;
}


vector<EulerAngles> GetNoisyOrientations(
	const vector<EulerAngles>& orientations,
	const array<double, 3> mean,
	const array<double, 3> sigma)
{
	random_device rd{};
	mt19937 gen{ rd() };
	normal_distribution<double>
		dr(mean[0], sigma[0]),
		dp(mean[1], sigma[1]),
		dy(mean[2], sigma[2]);

	vector<EulerAngles> noisy_orientations;

	for (size_t i = 0; i < orientations.size(); i++)
	{

		noisy_orientations.push_back({
			orientations[i].roll + dr(gen),
			orientations[i].pitch + dp(gen),
			orientations[i].yaw + dy(gen)
			});
	}
	return noisy_orientations;
}

vector<Unigine::Math::quat> GetNoisyOrientationsQuat(
	const vector<Unigine::Math::quat>& orientations,
	const array<double, 3> mean,
	const array<double, 3> sigma)
{
	random_device rd{};
	mt19937 gen{ rd() };
	normal_distribution<float>
		dr(mean[0], sigma[0]),
		dp(mean[1], sigma[1]),
		dy(mean[2], sigma[2]);

	vector<Unigine::Math::quat> noisy_orientations;

	for (size_t i = 0; i < orientations.size(); i++)
	{
		EulerAngles euler_angles = ToEulerAngles(orientations[i]);
		//std::cout << "was:" << euler_angles.roll << " " << euler_angles.pitch << " " << euler_angles.yaw << std::endl;
		euler_angles.roll += float(dr(gen));
		euler_angles.pitch += float(dp(gen));
		euler_angles.yaw += float(dy(gen));
		//std::cout << "is:" << euler_angles.roll << " " << euler_angles.pitch << " " << euler_angles.yaw << std::endl;
		noisy_orientations.push_back(ToQuaternion(euler_angles));

		//std::cout << "quat was: " << orientations[i].x << " " <<
		//	orientations[i].y << " " << orientations[i].z << " " << orientations[i].w << std::endl;

		//std::cout << "quat is: " << noisy_orientations[i].x << " " <<
		//	noisy_orientations[i].y << " " << noisy_orientations[i].z << " " << noisy_orientations[i].w << std::endl;
	}
	return noisy_orientations;
}
