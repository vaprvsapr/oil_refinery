#include "ScreenshotMaker.h"
#include <UnigineConsole.h>
#include <UnigineInput.h>
#include <UnigineGame.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include "Orientation.h"

using namespace Unigine;

void ScreenshotMaker::SetSize(Math::ivec2 _size)
{
	size = _size;
}

void ScreenshotMaker::SetSaveDir(std::string _save_dir)
{
	save_dir = _save_dir;
}

void ScreenshotMaker::SetFOV(float FOV)
{
	vertical_FOV = FOV;
}

void ScreenshotMaker::InitConstructor()
{
	image = Unigine::Image::create();
	viewport = Unigine::Viewport::create();
	viewport->setSkipFlags(Unigine::Viewport::SKIP_VISUALIZER);

	Unigine::Console::setOnscreen(1);
	//Unigine::Console::onscreenMessageLine(
	//	Unigine::Math::vec4_green, "Screenshot class creation.");
}

ScreenshotMaker::ScreenshotMaker()
{
	InitConstructor();
}

ScreenshotMaker::ScreenshotMaker(
	Unigine::Math::ivec2 _size,
	Format _format,
	std::pair<
	std::vector<Unigine::Math::Vec3>,
	std::vector<Unigine::Math::quat>> _trajectory
) : size(_size), format(_format), trajectory(_trajectory)
{
	InitConstructor();
}

ScreenshotMaker::ScreenshotMaker(
	std::pair<
	std::vector<Unigine::Math::Vec3>,
	std::vector<Unigine::Math::quat>> _trajectory
) : trajectory(_trajectory) 
{
	InitConstructor();
}

void ScreenshotMaker::SetTrajectory(std::pair<
	std::vector<Unigine::Math::Vec3>,
	std::vector<Unigine::Math::quat>> _trajectory)
{
	trajectory = _trajectory;
}

bool ScreenshotMaker::IsScreenshotBeingTaken()
{
	return (state != STATE_DONE);
}

void ScreenshotMaker::TakeScreenshot()
{
	warm_up_count = 0;
	state = STATE_WARMUP;
}

void ScreenshotMaker::TakeScreenshots()
{
	Console::onscreenMessageLine(Math::vec4_green, "Taking screenshots...");
	is_on_trajectory = true;
}

void ScreenshotMaker::Update()
{

	Unigine::Console::setOnscreen(1);
	auto player = Game::getPlayer();
	player->setFov(vertical_FOV / 3.141592 * 180);
	player->getCamera()->setFov(vertical_FOV / 3.141592 * 180);

	if (!player)
	{
		state = STATE_DONE;
		Console::onscreenMessageLine(
			Math::vec4_red, "No active camera.");
		return;
	}

	if ((state == STATE_DONE && Input::isKeyPressed(Input::KEY_SPACE)))
	{
		warm_up_count = 0;
		state = STATE_WARMUP;
		Console::onscreenMessageLine(
			Math::vec4_green, "Warming up...");
	}

	if (state == STATE_WARMUP)
	{
		viewport->setMode(Render::getViewportMode());
	

		if (warm_up_count == 0)
		{
			// First frame we render with velocity 
			// buffer turned off to avoid temporal effects artifacts
			viewport->appendSkipFlags(Viewport::SKIP_VELOCITY_BUFFER);
			viewport->renderImage2D(
				player->getCamera(), image, size.x, size.y);
			viewport->removeSkipFlags(Viewport::SKIP_VELOCITY_BUFFER);
		}
		else
		{
			// We temporarily set exposure adaptation time 
			// to 0, otherwise the image may be too dark
			float exposure_adaptation = 
				Render::getExposureAdaptation();
			Render::setExposureAdaptation(0.0f);
			viewport->renderImage2D(
				player->getCamera(), image, size.x, size.y);
			Render::setExposureAdaptation(exposure_adaptation);			
		}

		warm_up_count++;
		if (warm_up_count == warmup_frames)
			state = STATE_SAVE;
	}

	if (state == STATE_SAVE)
	{
		if (format == Format::jpg)
		{
			if (image->getFormat() == Image::FORMAT_RGBA8)
				image->convertToFormat(Image::FORMAT_RGB8);
			else if (image->getFormat() == Image::FORMAT_RGBA16F)
				image->convertToFormat(Image::FORMAT_RGB16F);
		}

		auto str_formats = String::split("tga,png,jpg", ",");

		std::stringstream ss;
		ss << std::setprecision(5)
			<< "x" << Game::getPlayer()->getWorldPosition().x
			<< "y" << Game::getPlayer()->getWorldPosition().y
			<< "z" << Game::getPlayer()->getWorldPosition().z
			<< "r" << ToEulerAngles(Game::getPlayer()->getWorldRotation()).roll
			<< "p" << (3.141592 / 2 - ToEulerAngles(Game::getPlayer()->getWorldRotation()).pitch)
			<< "y" << (3.141592 / 2 + ToEulerAngles(Game::getPlayer()->getWorldRotation()).yaw)
			<< "fov" << 2 * atan((float(size.x) / size.y) * 
				tan(Game::getPlayer()->getFov() / 360 * 3.141592));

		std::string camera_name = std::to_string(screenshot_count) +
			"CAM" /*+ ss.str()*/ + "." + str_formats[format];
		std::string str = path_to_save_dir + save_dir + "/" + camera_name;

		String fullName = String::format(str.c_str());

		image->save(fullName.get());
		Console::onscreenMessageLine(
			Math::vec4_green, "%s saved.", fullName.get());

		if (screenshot_count == 0)
			fout.open(path_to_save_dir + save_dir + "/trajectory.txt", ios::app);
		else
			fout.open(path_to_save_dir + save_dir + "/trajectory.txt", ios::app);

		fout << setprecision(5) << camera_name << " "
			<< Game::getPlayer()->getWorldPosition().x << " "
			<< Game::getPlayer()->getWorldPosition().y << " "
			<< Game::getPlayer()->getWorldPosition().z << " " << "0.1" << " "
			<< (ToEulerAngles(Game::getPlayer()->getWorldRotation()).yaw) / 3.141592 * 180 << " "
			<< (ToEulerAngles(Game::getPlayer()->getWorldRotation()).pitch) / 3.141592 * 180 << " "
			<< ToEulerAngles(Game::getPlayer()->getWorldRotation()).roll / 3.141592 * 180 << " 0.1" << '\n';
		fout.close();

		cout << camera_name << " "
			<< Game::getPlayer()->getWorldPosition().x << " "
			<< Game::getPlayer()->getWorldPosition().y << " "
			<< Game::getPlayer()->getWorldPosition().z << " "
			<< (ToEulerAngles(Game::getPlayer()->getWorldRotation()).yaw) / 3.141592 * 180 << " "
			<< (ToEulerAngles(Game::getPlayer()->getWorldRotation()).pitch) / 3.141592 * 180 << " "
			<< ToEulerAngles(Game::getPlayer()->getWorldRotation()).roll / 3.141592 * 180 << '\n';

		screenshot_count++;
		state = STATE_DONE;
	}

	if (is_on_trajectory && state == STATE_DONE)
	{
		if (trajectory_count < trajectory.first.size())
		{
			if (state == STATE_DONE)
			{
				player->setWorldPosition(
					trajectory.first[trajectory_count]
				);
				player->setWorldRotation(
					trajectory.second[trajectory_count]
				);
				trajectory_count++;

				warm_up_count = 0;
				state = STATE_WARMUP;
				Console::onscreenMessageLine(
					Math::vec4_green, "Warming up...");
			}
		}
		else
		{
			state = STATE_DONE;
			is_on_trajectory = false;
			trajectory_count = 0;
			fout.close();
		}
	}

	if (screenshot_count == trajectory.first.size())
		screenshot_count = 0;
}

