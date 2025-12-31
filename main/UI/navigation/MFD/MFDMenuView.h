#pragma once

#include <array>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/navigation/menu/menuView.h>
#include <UI/navigation/menu/menuViewButton.h>
#include <resources/images.h>

namespace pizda {
	using namespace YOBA;

	class MFDModeMenuViewButton : public MenuViewButton {
		public:
			explicit MFDModeMenuViewButton(const Image* image, std::wstring_view text, SettingsPersonalizationMFDToolbarMode mode);

		protected:
			void onClick() override;

		private:
			SettingsPersonalizationMFDToolbarMode _mode;
	};

	class MFDMenuView : public MenuView {
		public:
			explicit MFDMenuView();

			MenuViewButton
				PFDButton = { &resources::images::menuIconMFDPFD, L"PFD" },
				NDButton = { &resources::images::menuIconMFDND, L"ND" };

			MFDModeMenuViewButton
				autopilotButton { &resources::images::menuIconMFDAutopilot, L"A/P", SettingsPersonalizationMFDToolbarMode::autopilot },
				pressureButton { &resources::images::menuIconMFDBaro, L"BARO", SettingsPersonalizationMFDToolbarMode::baro },
				lightsButton { &resources::images::menuIconMFDLights, L"Lights", SettingsPersonalizationMFDToolbarMode::lights };

			std::array<MFDModeMenuViewButton*, 3> modeButtons {
				&autopilotButton,
				&pressureButton,
				&lightsButton
			};

			const Route* getRoute() override;

		private:

	};
}