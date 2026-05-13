#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>
#include "UI/theme.h"
#include "config.h"
#include "resources/images.h"

#include "UI/navigation/MFD/toolbar/toolbar.h"
#include "UI/navigation/MFD/toolbar/camera/cameraControls.h"
#include "UI/navigation/MFD/toolbar/toolbarButton.h"

namespace pizda {
	using namespace YOBA;

	class CameraToolbar : public RowToolbar {
		public:
			CameraToolbar();

		protected:
			void onTick() override;

		private:
			CameraControl _pitch;
			CameraControl _yaw;
			ImageToolbarButton _reset { L"Reset", &resources::images::menuIconMFDCameraResetImage };
	};
}