#include "cameraToolbar.h"

#include "rc.h"

namespace pizda {
	CameraToolbar::CameraToolbar() :
		_pitch({ config::camera::pitchAngleMinDeg, config::camera::pitchAngleMaxDeg, L"Pitch", &RC::getInstance().getRemoteData().camera.pitchAngleDeg }),
		_yaw({ config::camera::yawAngleMinDeg, config::camera::yawAngleMaxDeg, L"Yaw", &RC::getInstance().getRemoteData().camera.yawAngleDeg })
	{
		row += &_pitch;
		row += &_yaw;

		// Reset
		_reset.onPressed = [] {
			auto& rc = RC::getInstance();

			rc.getRemoteData().camera.pitchAngleDeg = 0;
			rc.getRemoteData().camera.yawAngleDeg = 0;

			rc.getTransceiver().enqueueAuxiliary(RemoteAuxiliaryPacketType::camera);

			rc.playFeedback();
		};

		row += &_reset;
	}

	void CameraToolbar::onTick() {
		RowToolbar::onTick();

		const auto& rd = RC::getInstance().getRemoteData();

		if (!_pitch.seven.isPointerOver())
			_pitch.seven.setValue(rd.camera.pitchAngleDeg);

		if (!_yaw.seven.isPointerOver())
			_yaw.seven.setValue(rd.camera.yawAngleDeg);
	}
}
