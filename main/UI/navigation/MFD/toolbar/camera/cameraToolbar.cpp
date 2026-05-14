#include "cameraToolbar.h"

#include "rc.h"

namespace pizda {
	CameraToolbar::CameraToolbar() :
		_pitch({ config::camera::pitchMinDeg, config::camera::pitchMaxDeg, L"Pitch", &RC::getInstance().getSettings().controls.cameraPitchDeg }),
		_yaw({ config::camera::yawMinDeg, config::camera::yawMaxDeg, L"Yaw", &RC::getInstance().getSettings().controls.cameraYawDeg })
	{
		row += &_pitch;
		row += &_yaw;

		// Reset
		_reset.onPressed = [] {
			auto& rc = RC::getInstance();

			rc.getSettings().controls.cameraPitchDeg = 0;
			rc.getSettings().controls.cameraYawDeg = 0;
			rc.getSettings().controls.scheduleWrite();

			rc.getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::camera);

			rc.playFeedback();
		};

		row += &_reset;
	}

	void CameraToolbar::onTick() {
		RowToolbar::onTick();

		auto& rc = RC::getInstance();

		if (!_pitch.seven.isPointerOver())
			_pitch.seven.setValue(rc.getSettings().controls.cameraPitchDeg);

		if (!_yaw.seven.isPointerOver())
			_yaw.seven.setValue(rc.getSettings().controls.cameraYawDeg);
	}
}
