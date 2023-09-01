#ifndef __APP_WORLD_LOGIC_H__
#define __APP_WORLD_LOGIC_H__

#include <UnigineLogic.h>
#include <UnigineStreams.h>
#include "Framework/Components/VRPlayerSpawner.h"
#include "Mode.h"
#include "Demo/Logic/GeneratorLogic.h"
#include "Demo/Logic/UdhLogic.h"
#include "ScreenshotMaker.h"

class AppWorldLogic : public Unigine::WorldLogic
{
private:
	GeneratorLogic *generator_logic;
	UdhLogic *udh_logic;
	Mode *telemetry = nullptr;

	bool interactive_player_init;
	VRPlayerSpawner *spawner;
	PlayerDummyPtr cinematic_player;

	void exitMode(Mode *& mode);

	ScreenshotMaker screenshot_maker;
	std::vector<float> coordinate_noise = { 0, 2, 4, 6, 8, 10, 12};
	std::vector<float> angles_noise = {3.141592 / 180 * 21, 3.141592 / 180 * 24, 3.141592 / 180 * 27};
	std::vector<float> vo = {  0.7, 0.8 };
	std::vector<float> ho = { 0.3, 0.4, 0.5, 0.6, 0.7,0.8 };
	std::vector<float> time_step = { 2, 3, 4, 5, 6.5, 8};
	std::vector<float> height = { 150, 175, 200 };
	int height_cnt = 0;
	int vo_cnt = 0;
	int ho_cnt = 0;
	int time_step_cnt = 0;
	int crd_cnt = 0;
	int ang_cnt = 0;
	bool working = false;
public:
	void setFreeMode();
	void setGeneratorInteractiveMode(int training_mode);
	void setUdhInteractiveMode(int training_mode);
	void setTelemetryMode();

public:
	AppWorldLogic();
	
	int init() override;
	int update() override;
	int shutdown() override;
};

#endif // __APP_WORLD_LOGIC_H__
