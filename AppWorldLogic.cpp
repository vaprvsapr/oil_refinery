#include "AppWorldLogic.h"
#include <UnigineInterpreter.h>
#include <UnigineInterface.h>
#include <UnigineConsole.h>
#include <UnigineGame.h>
#include <UnigineEditor.h>

#include "Telemetry.h"
#include "CreateTrajectory.h"
#include "GetNoisy.h"

#include <iostream>
#include <sstream>
#include <iomanip>

AppWorldLogic::AppWorldLogic()
{
	generator_logic = nullptr;
	udh_logic = nullptr;
}

int AppWorldLogic::init()
{
	Unigine::Console::run("render_auxiliary 1");

	interactive_player_init = VRPlayer::isVRPluginLoaded() ? true : false;

	NodePtr spawner_node = World::getNodeByName("spawn_point");
	if (!spawner_node)
	{
		Log::error("APP WORLD LOGIC ERROR: spawn point not found\n");
		return 0;
	}

	spawner = ComponentSystem::get()->getComponent<VRPlayerSpawner>(spawner_node);
	if (!spawner)
	{
		Log::error("APP WORLD LOGIC ERROR: spawn point node not contain VRPLayerSpawner component\n");
		return 0;
	}

	NodePtr cinematic_node = World::getNodeByName("cinematic_player");
	if (!cinematic_node)
	{
		Log::error("APP WORLD LOGIC ERROR: cinematic player not found\n");
		return 0;
	}

	cinematic_player = checked_ptr_cast<PlayerDummy>(cinematic_node);
	if (!cinematic_player)
	{
		Log::error("APP WORLD LOGIC ERROR: can't cast cinematic player from cinematic node\n");
		return 0;
	}

	NodePtr logic_layer = World::getNodeByName("logic_layer");
	if (!logic_layer)
	{
		Log::error("APP WORLD LOGIC ERROR: logic layer not found\n");
		return 0;
	}

	generator_logic = ComponentSystem::get()->getComponentInChildren<GeneratorLogic>(logic_layer);
	if (!generator_logic)
	{
		Log::error("APP WORLD LOGIC ERROR: generator logic component not found\n");
		return 0;
	}

	udh_logic = ComponentSystem::get()->getComponentInChildren<UdhLogic>(logic_layer);
	if (!udh_logic)
	{
		Log::error("APP WORLD LOGIC ERROR: udh logic component not found\n");
		return 0;
	}

	Input::setMouseHandle(Input::MOUSE_HANDLE_SOFT);

	float FOV = 0.762;
	Unigine::Math::ivec2 size = { 3000	, 2000 };
	screenshot_maker.SetSize(size);
	screenshot_maker.SetFOV(FOV);
	screenshot_maker.SetSaveDir("SONY 3000x2000HFOV62/free_fly");

	return 1;
}

void AppWorldLogic::setFreeMode()
{
	std::cout << "setFreeMode()" << std::endl;
	// disable other modes
	exitMode(telemetry);
	//Game::setPlayer(cinematic_player);
	Input::setMouseHandle(Input::MOUSE_HANDLE_GRAB);
}

void AppWorldLogic::setGeneratorInteractiveMode(int training_mode)
{
	std::cout << "setGeneratorInteractiveMode()" << std::endl;
	// disable other modes
	exitMode(telemetry);

	// enable VRPlayer
	if (VRPlayer::get())
		VRPlayer::get()->getPlayer()->setEnabled(true);

	if (udh_logic && udh_logic->isActive())
		udh_logic->stop();

	// enable interactive mode
	if (!generator_logic)
		return;

	if (generator_logic->isActive())
		generator_logic->stop();

	generator_logic->start(training_mode);
}

void AppWorldLogic::setUdhInteractiveMode(int training_mode)
{
	std::cout << "setUdhInteractiveMode()" << std::endl;
	// disable other modes
	exitMode(telemetry);

	// enable VRPlayer
	if (VRPlayer::get())
		VRPlayer::get()->getPlayer()->setEnabled(true);

	if (generator_logic && generator_logic->isActive())
		generator_logic->stop();

	// enable interactive mode
	if (!udh_logic)
		return;

	if (udh_logic->isActive())
		udh_logic->stop();

	udh_logic->start(training_mode);
}


void AppWorldLogic::setTelemetryMode()
{
	screenshot_maker.TakeScreenshots();
	working = true;
	Game::setPlayer(cinematic_player);
}

void AppWorldLogic::exitMode(Mode *& mode)
{
	if (!mode)
		return;

	mode->shutdown();
	delete mode;
	mode = nullptr;
}

int AppWorldLogic::update()
{
	if (!interactive_player_init && spawner->isInitialized())
	{
		Game::setPlayer(cinematic_player);
		if (VRPlayer::get())
			VRPlayer::get()->getPlayer()->setEnabled(false);

		interactive_player_init = true;
	}

	if (telemetry)
		telemetry->update();

	if (Input::isKeyDown(Input::KEY_F8))
		Console::run("world_reload");

	float FOV = 0.762;
	Unigine::Math::ivec2 size = { 3000, 2000 };
	screenshot_maker.SetSize(size);
	screenshot_maker.SetFOV(FOV);

	/*if(!screenshot_maker.is_on_trajectory && working && vo_cnt < vo.size())
	{
		cout << "OK";
		auto trajectory_mission_planner = GetTrajectoryFromArdupilotLogTrueOverlap(
			"D:/important/logs/SONY 3000x2000/Trajectories/hn=0, vn=0/ho=0." + to_string(int(ho[ho_cnt] * 10)) + ".txt",
			vo[vo_cnt], { 2000, 3000 }, FOV);
		screenshot_maker.SetSaveDir("SONY 3000x2000HFOV62/hn=0, vn=0/ho=0." + to_string(int(ho[ho_cnt] * 10)) + ", vo=0." + to_string(int(vo[vo_cnt++] * 10)));
		screenshot_maker.is_on_trajectory = true;
		screenshot_maker.SetTrajectory(trajectory_mission_planner);
	}
	if (vo_cnt >= vo.size())
	{
		vo_cnt = 0;
		ho_cnt++;
	}
	if (ho_cnt >= ho.size())
		working = false;*/

	//if (!screenshot_maker.is_on_trajectory && working && vo_cnt < vo.size())
	//{
	//	cout << "OK";
	//	auto trajectory_mission_planner = GetTrajectoryFromArdupilotLogTrueOverlap(
	//		"D:/important/logs/SONY 3000x2000/Trajectories/height/height=" + to_string(int(height[height_cnt])) + ".txt",
	//		vo[vo_cnt], { 2000, 3000 }, FOV);
	//	screenshot_maker.SetSaveDir("SONY 3000x2000HFOV62/height/adaptive_time_step/ho=0." + to_string(int(ho[ho_cnt] * 10)) + ", vo=0." + to_string(int(vo[vo_cnt++] * 10)) + ", height=" + to_string(height[height_cnt]));
	//	screenshot_maker.is_on_trajectory = true;
	//	screenshot_maker.SetTrajectory(trajectory_mission_planner);
	//}
	//if (vo_cnt >= vo.size())
	//{
	//	vo_cnt = 0;
	//	height_cnt++;
	//}
	//if (height_cnt >= height.size())
	//	working = false;

	if (!screenshot_maker.is_on_trajectory && working && vo_cnt < vo.size())
	{
		cout << "OK";
		auto trajectory_mission_planner = GetTrajectoryFromArdupilotLog(
			"D:/important/logs/SONY 3000x2000/Trajectories/hn=7.5, vn=7.5/ho=0." + to_string(int(ho[ho_cnt] * 10)) + ".txt", time_step[time_step_cnt]);
		screenshot_maker.SetSaveDir("SONY 3000x2000HFOV62/fixed_time_step/pn=7.5/ho=0." + to_string(int(ho[ho_cnt] * 10)) + ", ts=" + to_string(time_step[time_step_cnt]));
		screenshot_maker.is_on_trajectory = true;
		screenshot_maker.SetTrajectory(trajectory_mission_planner);
		time_step_cnt++;
	}
	if (time_step_cnt >= time_step.size())
	{
		time_step_cnt = 0;
		ho_cnt++;
	}
	if (ho_cnt >= ho.size())
		working = false;

	//if (!screenshot_maker.is_on_trajectory && working && vo_cnt < vo.size())
	//{
	//	cout << "OK";
	//	auto trajectory_mission_planner = GetTrajectoryFromArdupilotLog(
	//		"D:/important/logs/SONY 3000x2000/Trajectories/height/height=" + to_string(int(height[height_cnt])) + ".txt", time_step[time_step_cnt]);
	//	screenshot_maker.SetSaveDir("SONY 3000x2000HFOV62/height/fixed_time_step_low/ho=0." + to_string(int(ho[ho_cnt] * 10)) + ", ts=" + to_string(time_step[time_step_cnt]) + ", height=" + to_string(height[height_cnt]));
	//	screenshot_maker.is_on_trajectory = true;
	//	screenshot_maker.SetTrajectory(trajectory_mission_planner);
	//	time_step_cnt++;
	//}
	//if (time_step_cnt >= time_step.size())
	//{
	//	time_step_cnt = 0;
	//	height_cnt++;
	//}
	//if (ho_cnt >= ho.size())
	//	working = false;

	screenshot_maker.Update();

	return 1;
}

int AppWorldLogic::shutdown()
{
	generator_logic = nullptr;
	udh_logic = nullptr;

	return 1;
}
