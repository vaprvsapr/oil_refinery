#include "ObjMouseMovable.h"
#include <UnigineInput.h>


REGISTER_COMPONENT(ObjMouseMovable);


void ObjMouseMovable::grabIt(VRPlayer* player, int hand_num)
{
	if (VRPlayer::isVRPluginLoaded())
	{
		ObjMovable::grabIt(player, hand_num);
		return;
	}

	auto gui = WindowManager::getMainWindow()->getSelfGui();

	local_transform = player->getHead()->getIWorldTransform() * node->getWorldTransform();
	grab_x = gui->getWidth() / 2.0f;
	grab_y = gui->getHeight() / 2.0f;
	offset = Vec3_zero;

	ObjMovable::grabIt(player, hand_num);
}

void ObjMouseMovable::holdIt(VRPlayer *player, int hand_num)
{
	if (VRPlayer::isVRPluginLoaded())
	{
		ObjMovable::holdIt(player, hand_num);
		return;
	}

	if (use_handy_pos)
	{
		auto gui = WindowManager::getMainWindow()->getSelfGui();
		float ifps = Game::getIFps() / Game::getScale();

		ivec2 mouse_pos = Input::getMousePosition();
		offset.x = (mouse_pos.x - grab_x) / gui->getWidth();
		offset.y = (grab_y - mouse_pos.y) / gui->getHeight();
		offset.z -= 0.02f * Input::getMouseWheel();

		setTransformRotation(local_transform, handy_rot_quat);
		float k = saturate(handy_pos_factor * ifps);
		setTransformPosition(local_transform, lerp(local_transform.getTranslate(), (Vec3)handy_pos.get() + offset, k));

		Mat4 result = player->getHead()->getWorldTransform() * local_transform;
		node->setWorldTransform(result);
	}

	VRInteractable::holdIt(player, hand_num);
}

void ObjMouseMovable::movable_init()
{
	handy_rot_quat = Math::quat(handy_rot_euler.get().x, handy_rot_euler.get().y, handy_rot_euler.get().z);
}