#include "cameraToolbar.h"

#include "rc.h"

namespace pizda {
	CameraToolbar::CameraToolbar() :
		_pitch({ L"Ailerons", &RC::getInstance().getRemoteData().camera.pitchAngleDeg }),
		_yaw({ L"Elevator", &RC::getInstance().getRemoteData().camera.yawAngleDeg })
	{
		row += &_pitch;
		row += &_yaw;
	}
}