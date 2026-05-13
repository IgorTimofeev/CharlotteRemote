#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "UI/theme.h"
#include "config.h"

#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "UI/navigation/MFD/toolbar/camera/cameraControls.h"

namespace pizda {
	using namespace YOBA;

	class CameraToolbar : public RowToolbar {
		public:
			CameraToolbar();
			
		private:
			CameraControl<config::camera::pitchAngleMinDeg, config::camera::pitchAngleMaxDeg> _pitch;
			CameraControl<config::camera::yawAngleMinDeg, config::camera::yawAngleMaxDeg> _yaw;
	};
}