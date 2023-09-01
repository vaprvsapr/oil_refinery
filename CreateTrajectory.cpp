#include "CreateTrajectory.h"

pair<vector<Unigine::Math::Vec3>,
	vector<Unigine::Math::quat>> CreateTackTrajectory(
	Coordinate2D initial_point,
	Size2D delta,
	Size2D overlap, // {0.2, 0.3}
	double pitch,
	double height,
	double vertical_field_of_view,
	ImageSize image_size,
	double sea_level
)
{
	MyCamera camera{
		{0, 0, height},
		{0, 3.141592 / 2 - pitch, 0},
		vertical_field_of_view,
		image_size
	};

	Coordinate3D
		top_left_3d = GetPixelCoordinate(camera,
			{ 0, 0 }),
		top_right_3d = GetPixelCoordinate(camera,
			{ 0, camera.image_size.width }),
		bottom_left_3d = GetPixelCoordinate(camera,
			{ camera.image_size.height, 0 }),
		bottom_right_3d = GetPixelCoordinate(camera,
			{ camera.image_size.height,
			camera.image_size.width });

	Coordinate2D
		top_left = Projection(camera.position, top_left_3d),
		top_right = Projection(camera.position, top_right_3d),
		bottom_left = Projection(camera.position, bottom_left_3d),
		bottom_right = Projection(camera.position, bottom_right_3d);

	Size2D rectangle_size = {
		abs(top_left.x - bottom_left.x),
		abs(bottom_left.y - bottom_right.y) / 2 + abs(top_left.y - top_right.y) / 2
	};

	Size2D shift = {
		rectangle_size.height * (1 - overlap.height),
		rectangle_size.width * (1 - overlap.width)
	};

	size_t n_height = ceil(delta.height / shift.height);
	size_t n_width = ceil(delta.width / shift.width);

	Size2D alignment = {
		(rectangle_size.height + (n_height - 1) * shift.height - delta.height) / 2,
		(rectangle_size.width + (n_width - 1) * shift.width - delta.width) / 2
	};

	vector<Unigine::Math::Vec3> positions;
	vector<Unigine::Math::quat> orientations;

	for (size_t j = 0; j < n_width; j++)
	{
		for (size_t i = 0; i < n_height; i++)
		{
			if ((j % 2) == 0)
			{
				positions.push_back({
					float(initial_point.x + i * shift.height +
					height / tan(3.141592 - pitch) - bottom_left.x - alignment.height - height / tan(3.141592 - pitch)),
					float(initial_point.y + j * shift.width +
					rectangle_size.width / 2 - alignment.width),
					float(height + sea_level)
					});
				orientations.push_back(ToQuaternion({ 0, pitch, -3.141592 / 2 }));
			}
			if ((j % 2) == 1)
			{
				positions.push_back({
					float(initial_point.x + delta.height - i * shift.height -
					height / tan(3.141592 - pitch) + bottom_left.x + alignment.height + height / tan(3.141592 - pitch)),
					float(initial_point.y + j * shift.width +
					rectangle_size.width / 2 - alignment.width),
					float(height + sea_level)
					});
				orientations.push_back(ToQuaternion({ 0, pitch, 3.141592 / 2 }));
			}
		}
	}
	return make_pair(positions, orientations);
}

pair<vector<Coordinate3D>, vector<Unigine::Math::quat>> CreateTrajectoryForGeoscan201(
	Coordinate2D initial_point,
	Size2D delta,
	Size2D overlap,
	double horizontal_field_of_view,
	ImageSize image_size,
	size_t max_frame_rate,
	double spatial_resolution
)
{
	pair<double, double> speed_range = { 20, 36 }; // горизонтальная скорость в м/с
	double height =
		spatial_resolution / (tan(horizontal_field_of_view / 2) -
			tan(horizontal_field_of_view / 2 -
				horizontal_field_of_view / image_size.width));
	height = height * 0.9; // закладываем дополнительную точность
	height = floor(height); // округляем до целого



	MyCamera camera{
	{0, 0, height},
	{0, 3.141592 / 2, 0}, // надир
	horizontal_field_of_view,
	image_size
	};

	Coordinate3D
		top_left_3d = GetPixelCoordinate(camera,
			{ 0, 0 }),
		top_right_3d = GetPixelCoordinate(camera,
			{ 0, camera.image_size.width }),
		bottom_left_3d = GetPixelCoordinate(camera,
			{ camera.image_size.height, 0 }),
		bottom_right_3d = GetPixelCoordinate(camera,
			{ camera.image_size.height,
			camera.image_size.width });

	Coordinate2D
		top_left = Projection(camera.position, top_left_3d),
		top_right = Projection(camera.position, top_right_3d),
		bottom_left = Projection(camera.position, bottom_left_3d),
		bottom_right = Projection(camera.position, bottom_right_3d);

	Size2D rectangle_size = {
		abs(top_left.x - bottom_left.x),
		abs(top_left.y - top_right.y)
	};


	Size2D desired_shift = {
	rectangle_size.height * (1 - overlap.height),
	rectangle_size.width * (1 - overlap.width)
	};

	double speed = 0;
	size_t min_frame_rate = max_frame_rate;
	for (size_t frame_rate = max_frame_rate; frame_rate > 0; frame_rate--)
	{
		pair<double, double> shift_range = {
			speed_range.first / frame_rate,
			speed_range.second / frame_rate };
		if ((desired_shift.height > shift_range.first))
		{
			min_frame_rate = frame_rate;
			speed = desired_shift.height / frame_rate;
			speed = speed > speed_range.second ? speed_range.second : speed;
		}
	}

	//speed = floor(speed * 100) / 100;

	Size2D shift = {
		speed * min_frame_rate,
		desired_shift.width
	};


	size_t n_height = ceil(delta.height / shift.height);
	size_t n_width = ceil(delta.width / shift.width);

	Size2D alignment = {
		(n_height * shift.height - delta.height) / 2,
		(n_width * shift.width - delta.width) / 2
	};

	vector<Coordinate3D> positions;
	vector <Unigine::Math::quat> orientations;

	for (size_t j = 0; j < n_width; j++)
	{
		for (size_t i = 0; i < n_height; i++)
		{
			if ((j % 2) == 0)
			{
				positions.push_back({
					initial_point.x + i * shift.height -
					bottom_left.x - alignment.height,
					initial_point.y + j * shift.width +
					rectangle_size.width / 2 - alignment.width,
					height
					});
				orientations.push_back(ToQuaternion({ 0, 3.141592 / 2, 0 }));
			}
			if ((j % 2) == 1)
			{
				positions.push_back({
					initial_point.x + delta.height - i * shift.height +
					bottom_left.x + alignment.height,
					initial_point.y + j * shift.width +
					rectangle_size.width / 2 - alignment.width,
					height
					});
				orientations.push_back(ToQuaternion({ 0, 3.141592 / 2, 3.141592 }));
			}
		}
	}
	return make_pair(positions, orientations);
}

pair<vector<Unigine::Math::Vec3>, vector<Unigine::Math::quat>> GetTrajectoryFromArdupilotLog(string log, float time_step)
{
	ifstream input;
	input.open(log);

	string mavlink_local_position_ned_t = "mavlink_local_position_ned_t";
	int cnt = 0;
	string line;
	bool is_ahrs = 0;
	bool is_recording = 0;
	int initial_time = 0;
	size_t sec_cnt = 1;


	vector<Unigine::Math::Vec3> positions;
	vector<Unigine::Math::quat> orientations;

	while (!input.eof())
	{
		input.ignore(48);
		string line;
		input >> line;

		if (line == "mavlink_local_position_ned_t")
		{
			input.ignore(13);
			int time_boot_ms;
			input >> time_boot_ms;
			string x, y, z;
			float x_d, y_d, z_d;

			input.ignore(3);
			input >> x;
			auto iter = x.find(',');
			x[iter] = '.';
			stringstream ss_x(x);
			ss_x >> x_d;

			input.ignore(3);
			input >> y;
			iter = y.find(',');
			y[iter] = '.';
			stringstream ss_y(y);
			ss_y >> y_d;

			input.ignore(3);
			input >> z;
			iter = z.find(',');
			z[iter] = '.';
			stringstream ss_z(z);
			ss_z >> z_d;
			z_d *= -1;

			if (abs(z_d - 50) < 10)
			{
				is_recording = 1;
				initial_time = time_boot_ms;
			}
			is_ahrs = 1;

			if (is_recording && ((sec_cnt % int(time_step * 2)) == 0))
			{
				std::cout << "time_boot_ms:" << time_boot_ms << ", x:" << x_d << ", y:" << y_d << ", z:" << z_d << endl;
				positions.push_back({ y_d + 470, x_d + 496, z_d + 130 });
			}
			
		}
		else
		{
			if (line == "mavlink_ahrs2_t" && is_ahrs)
			{
				string rolls, pitchs, yaws;
				double roll, pitch, yaw;
				input.ignore(6);
				input >> rolls;
				auto iter = rolls.find(',');
				rolls[iter] = '.';
				stringstream ss_roll(rolls);
				ss_roll >> roll;
				input.ignore(7);
				input >> pitchs;
				iter = pitchs.find(',');
				pitchs[iter] = '.';
				stringstream ss_pitch(pitchs);
				ss_pitch >> pitch;
				input.ignore(5);
				input >> yaws;
				iter = yaws.find(',');
				yaws[iter] = '.';
				stringstream ss_yaw(yaws);
				ss_yaw >> yaw;
				input.ignore(1000, '\n');
				is_ahrs = 0;

				if (is_recording && (sec_cnt % int(time_step * 2)) == 0)
				{
					std::cout << "roll:" << roll << ", pitch:" << pitch << ", yaw:" << yaw << endl;
					orientations.push_back(ToQuaternion({ roll, pitch, -yaw }));
				}
				sec_cnt++;
			}
			else
				input.ignore(1000, '\n');
		}

	}
	return { positions, orientations };
}

#include "CalculateOverlap.h"

pair<vector<Unigine::Math::Vec3>, vector<Unigine::Math::quat>> GetTrajectoryFromArdupilotLogTrueOverlap(string log, float vertical_overlap, ImageSize image_size, float VFOV)
{
	ifstream input;
	input.open(log);

	string mavlink_local_position_ned_t = "mavlink_local_position_ned_t";
	int cnt = 0;
	string line;
	bool is_ahrs = 0;
	bool is_recording = 0;
	int initial_time = 0;
	size_t sec_cnt = 1;


	vector<Coordinate3D> positions;
	vector<EulerAngles> orientations;

	while (!input.eof())
	{
		input.ignore(48);
		string line;
		input >> line;

		if (line == "mavlink_local_position_ned_t")
		{
			input.ignore(13);
			int time_boot_ms;
			input >> time_boot_ms;
			string x, y, z;
			float x_d, y_d, z_d;

			input.ignore(3);
			input >> x;
			auto iter = x.find(',');
			x[iter] = '.';
			stringstream ss_x(x);
			ss_x >> x_d;

			input.ignore(3);
			input >> y;
			iter = y.find(',');
			y[iter] = '.';
			stringstream ss_y(y);
			ss_y >> y_d;

			input.ignore(3);
			input >> z;
			iter = z.find(',');
			z[iter] = '.';
			stringstream ss_z(z);
			ss_z >> z_d;
			z_d *= -1;

			if (abs(z_d - 100) < 10)
			{
				is_recording = 1;
				initial_time = time_boot_ms;
			}
			is_ahrs = 1;

			if (is_recording)
			{
				positions.push_back({ y_d + 470, x_d + 496, z_d });
			}

		}
		else
		{
			if (line == "mavlink_ahrs2_t" && is_ahrs)
			{
				string rolls, pitchs, yaws;
				double roll, pitch, yaw;
				input.ignore(6);
				input >> rolls;
				auto iter = rolls.find(',');
				rolls[iter] = '.';
				stringstream ss_roll(rolls);
				ss_roll >> roll;
				input.ignore(7);
				input >> pitchs;
				iter = pitchs.find(',');
				pitchs[iter] = '.';
				stringstream ss_pitch(pitchs);
				ss_pitch >> pitch;
				input.ignore(5);
				input >> yaws;
				iter = yaws.find(',');
				yaws[iter] = '.';
				stringstream ss_yaw(yaws);
				ss_yaw >> yaw;
				input.ignore(1000, '\n');
				is_ahrs = 0;

				if (is_recording)
				{
					orientations.push_back({roll, pitch, -yaw});
				}
				sec_cnt++;
			}
			else
				input.ignore(1000, '\n');
		}
	}
	vector<Unigine::Math::Vec3> positions_Unigine;
	vector<Unigine::Math::quat> orientations_Unigine;

	int prev_camera_cnt = 0;
	positions_Unigine.push_back({ float(positions[0].x), float(positions[0].y), float(positions[0].z + 130) });
	orientations_Unigine.push_back(ToQuaternion(orientations[0]));

	for (int camera_cnt = 1; camera_cnt < positions.size(); camera_cnt++)
	{
		MyCamera prev_camera
		{
			positions[prev_camera_cnt],
			{
				orientations[prev_camera_cnt].roll,
				orientations[prev_camera_cnt].pitch * (-1) + 3.141592 / 2,
				orientations[prev_camera_cnt].yaw + 3.141592 / 2
			},
			VFOV,
			image_size
		},
			current_camera
		{
			positions[camera_cnt],
			{
				orientations[camera_cnt].roll,
				orientations[camera_cnt].pitch * (-1) + 3.141592 / 2,
				orientations[camera_cnt].yaw + 3.141592 / 2
			},
			VFOV,
			image_size
		};

		cout << "camera_cnt: " << camera_cnt << ", pos: " << positions[camera_cnt].x << " "
			<< positions[camera_cnt].y << " "
			<< positions[camera_cnt].z << ", rpy: "
			<< orientations[camera_cnt].roll << " "
			<< orientations[camera_cnt].pitch * (-1) + 3.141592 / 2 << " "
			<< orientations[camera_cnt].yaw + 3.141592 / 2 << endl;
		
		

		double overlap = CalculateAreaOverlap(prev_camera, current_camera);
		if (overlap <= vertical_overlap)
		{
			
			positions_Unigine.push_back({ float(positions[camera_cnt - 1].x), float(positions[camera_cnt - 1].y), float(positions[camera_cnt - 1].z + 130) });
			orientations_Unigine.push_back(ToQuaternion(orientations[camera_cnt - 1]));
			prev_camera_cnt = camera_cnt - 1;
		}
	}
	
	return {positions_Unigine, orientations_Unigine};
}