#pragma once
#include <UniginePlayers.h>
#include <UnigineNodes.h>
#include <UnigineEditor.h>
#include <UnigineControls.h>
#include <UnigineWidgets.h>
#include "VRPlayer.h"

class VRPlayerPC : public VRPlayer
{
public:
	COMPONENT(VRPlayerPC, VRPlayer);
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);
	COMPONENT_POST_UPDATE(update_mouse_state, 1)
	COMPONENT_POST_UPDATE(post_update);
	COMPONENT_SHUTDOWN(shutdown);

	// property
	PROP_NAME("player_pc");
	PROP_AUTOSAVE(1);

	// parameters
	PROP_PARAM(Material, post_material)
	PROP_PARAM(Float, move_speed, 2.5f);	// speed of the player
	PROP_PARAM(Float, acceleration, 8.0f);	// acceleration of the player
	PROP_PARAM(Float, damping, 8.0f);		// damping of the player
	PROP_PARAM(Float, hand_length, 1.3f);	// uses for grab objects
	PROP_PARAM(Float, throw_force, 7.0f);	// start velocity of grabbed object
	PROP_PARAM(File, mouse_arrow_sprite);
	
	UNIGINE_INLINE static int isGamepadLastControl() { return last_used_control == 1; }
	UNIGINE_INLINE static void setLastControl(int gamepad) { last_used_control = gamepad; }

	void on_disable() override;

	// hacks (because En-2610)
	static int xpad_back;
	static int xpad_start;

	// player
	Unigine::PlayerPtr getPlayer();
	Unigine::PlayerActorPtr getActor();
	//int isEnabled();
	void setEnabled(bool enabled);
	void landPlayerTo(const Unigine::Math::Vec3 &position, const Unigine::Math::vec3 &direction) override;

	// hands
	int getNumHands() override;										// get count of hands
	Unigine::NodePtr getHandNode(int num) override;					// get hand's node
	int getHandDegreesOfFreedom(int num) override;					// get hand's degrees of freedom
	Unigine::Math::vec3 getHandLinearVelocity(int num) override;	// get speed of hand
	Unigine::Math::vec3 getHandAngularVelocity(int num) override;	// get angular speed of hand
	int getHandState(int num) override;								// get hand state (grab, hold, throw)
	const Unigine::NodePtr &getGrabNode(int num) const override;
	const Unigine::Vector<VRInteractable*> &getGrabComponents(int num) const override;
	const Unigine::Vector<VRInteractable*> &getSelectedComponents(int num) const override;
	Mat4 getHandyWorldTransform(int num) override;
	Mat4 getHandyIWorldTransform(int num) override;
	void grabObject(int num, ObjectPtr obj, float distance);
	void throwObject(int num);

	UNIGINE_INLINE Unigine::Math::vec3 getHandyPos(int num) override { return Unigine::Math::vec3(0, 0, 0); }
	UNIGINE_INLINE Unigine::Math::quat getHandyRot(int num) override { return Unigine::Math::quat(0, 0, 0); }

	int getControllerButton(int controller_num, int button) override;
	int getControllerButtonDown(int controller_num, int button) override;
	int getControllerButtonUp(int controller_num, int button) override;
	float getControllerAxis(int controller_num, int button) override;
	void vibrateController(int controller_num, float amplitude = 1.0f) override;

	virtual void update_button_states() override;

protected:
	// main loop
	void init();
	void update();
	void post_update();
	void shutdown();

private:
	void update_mouse_state();

	Unigine::PlayerActorPtr actor;
	Unigine::NodeDummyPtr hand;

	Unigine::ControlsPtr controls;
	Unigine::ControlsGamepadPtr xpad360;
	static int last_used_control; // 0 - keyboard/mouse, 1 - gamepad

	// controllers buttons
	int buttons_prev_state[BUTTON::COUNT];

	Unigine::WidgetSpritePtr mouse_arrow;
	bool need_hide_cursour = false;

	Unigine::Math::Vec3 last_hand_pos;
	Unigine::Math::Vec3 hand_offset;

	int last_grab_button_state = 0;
	int hand_state = 0;		// status of hand: free, grab, hold, throw, etc.
	int node_grabbed = 0;	// hand grab something?
	Unigine::NodePtr grab_node;		// what is it?
	Unigine::Vector<VRInteractable*> grab_components;
	Unigine::Vector<VRInteractable *> selected_components;

	Unigine::ObjectPtr null_selected, last_selected;
	Unigine::WorldIntersectionPtr intersection = Unigine::WorldIntersection::create();

	float saved_dof_blur = 0;
	float dof_dist = 0;					// smooth focal distance
	float dof_dist_factor = 5;			// speed of DOF adaptation
	float dof_min_dist = 5.0f;			// min distance of DOF Focal Distance
	float dof_max_dist = 2000;			// max distance of DOF Focal Distance
	float dof_near_dist = 0;			// current DOF near distance
	float dof_far_dist = 0;				// current DOF far distance

	int use_zoom = 1;					// use zoom by RMB?
	float zoom = 60;					// default value
	float zoom_factor = 10;				// speed of change zoom
	float zoom_min = 30;				// minimum fov (use zoom)
	float zoom_max = 60;				// maximum fov

	float mouse_sensitivity = 0.5f;
	float collision_height,
		collision_height_max,
		collision_height_min;

	void controls_update();
	void head_update();
	void grab_update();
	void zoom_update();
	void dof_update();
	void time_scale_update();
	void crouch_update();
	void disable_outline();
};