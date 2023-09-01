#pragma once

#include <UnigineComponentSystem.h>
#include <UnigineViewport.h>
#include <string>
#include <vector>
#include <fstream>

class ScreenshotMaker
{
	enum Format
	{
		tga = 0,
		png,
		jpg
	};

	enum State
	{
		STATE_WARMUP,
		STATE_SAVE,
		STATE_DONE,
	};

public:
	void Update();
	void TakeScreenshot();
	void TakeScreenshots();
	bool IsScreenshotBeingTaken();
	void SetTrajectory(std::pair<
		std::vector<Unigine::Math::Vec3>,
		std::vector<Unigine::Math::quat>> _trajectory);
	void SetSaveDir(std::string _save_dir);

	ScreenshotMaker();

	ScreenshotMaker(
		Unigine::Math::ivec2 _size,
		Format _format,
		std::pair<
		std::vector<Unigine::Math::Vec3>,
		std::vector<Unigine::Math::quat>> _trajectory
	);

	ScreenshotMaker(
		std::pair<
		std::vector<Unigine::Math::Vec3>,
		std::vector<Unigine::Math::quat>> _trajectory
	);

	void SetSize(Unigine::Math::ivec2 _size);
	void SetFOV(float FOV);

	bool is_on_trajectory = false;
private:
	double vertical_FOV = 60;
	void InitConstructor();

	Unigine::ImagePtr image;
	Unigine::ViewportPtr viewport;
	int screenshot_count = 0;

	State state = STATE_DONE;
	const int warmup_frames = 10;
	int warm_up_count = 0;

	std::string name_prefix = "screenshot";
	Unigine::Math::ivec2 size = { 1920, 1080 };
	Format format = jpg;

	std::pair<
		std::vector<Unigine::Math::Vec3>,
		std::vector<Unigine::Math::quat>> trajectory;
	int trajectory_count = 0;


	std::string save_dir = "screenshots";
	std::string path_to_save_dir = "D:/important/Unigine/Storage/";
	std::ofstream fout;
};

