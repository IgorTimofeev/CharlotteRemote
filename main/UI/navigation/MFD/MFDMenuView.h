#pragma once

#include <array>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/menu/menuView.h"
#include "UI/navigation/menu/menuViewButton.h"
#include "resources/images.h"

namespace pizda {
	using namespace YOBA;

	class MFDModeMenuViewButton : public MenuViewButton {
		public:
			explicit MFDModeMenuViewButton(const Image* image, std::string_view text, PersonalizationSettingsMFDToolbarMode mode);

		protected:
			void onClick() override;

		private:
			PersonalizationSettingsMFDToolbarMode _mode;
	};

	class MFDMenuView : public MenuView {
		public:
			explicit MFDMenuView();

			MFDModeMenuViewButton
				autopilotButton { &resources::images::menuIconMFDAutopilot, "A/P", PersonalizationSettingsMFDToolbarMode::autopilot },
				pressureButton { &resources::images::menuIconMFDBaro, "BARO", PersonalizationSettingsMFDToolbarMode::baro },
				trimButton { &resources::images::menuIconMFDTrim, "Trim", PersonalizationSettingsMFDToolbarMode::trim },
				lightsButton { &resources::images::menuIconMFDLights, "Lights", PersonalizationSettingsMFDToolbarMode::lights },
				cameraButton { &resources::images::menuIconMFDCameraImage, "Camera", PersonalizationSettingsMFDToolbarMode::camera };

			std::array<MFDModeMenuViewButton*, 5> modeButtons {
				&autopilotButton,
				&pressureButton,
				&trimButton,
				&lightsButton,
				&cameraButton
			};

			const Route* getRoute() override;

		private:

	};
}