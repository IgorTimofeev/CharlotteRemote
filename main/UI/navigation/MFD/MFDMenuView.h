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
			explicit MFDModeMenuViewButton(const Image* image, std::wstring_view text, PersonalizationSettingsMFDToolbarMode mode);

		protected:
			void onClick() override;

		private:
			PersonalizationSettingsMFDToolbarMode _mode;
	};

	class MFDMenuView : public MenuView {
		public:
			explicit MFDMenuView();

			MenuViewButton
				PFDButton = { &resources::images::menuIconMFDPFD, L"PFD" },
				NDButton = { &resources::images::menuIconMFDND, L"ND" };

			MFDModeMenuViewButton
				autopilotButton { &resources::images::menuIconMFDAutopilot, L"A/P", PersonalizationSettingsMFDToolbarMode::autopilot },
				pressureButton { &resources::images::menuIconMFDBaro, L"BARO", PersonalizationSettingsMFDToolbarMode::baro },
				trimButton { &resources::images::menuIconMFDTrim, L"Trim", PersonalizationSettingsMFDToolbarMode::trim },
				lightsButton { &resources::images::menuIconMFDLights, L"Lights", PersonalizationSettingsMFDToolbarMode::lights };

			std::array<MFDModeMenuViewButton*, 4> modeButtons {
				&autopilotButton,
				&pressureButton,
				&trimButton,
				&lightsButton
			};

			const Route* getRoute() override;

		private:

	};
}