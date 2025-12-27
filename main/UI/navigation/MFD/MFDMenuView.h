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
			explicit MFDModeMenuViewButton(const Image* image, std::wstring_view text, SettingsInterfaceMFDToolbarMode mode);

		protected:
			void onClick() override;

		private:
			SettingsInterfaceMFDToolbarMode _mode;
	};

	class MFDMenuView : public MenuView {
		public:
			explicit MFDMenuView();

			MenuViewButton
				PFDButton = { &resources::Images::menuIconMFDPFD, L"PFD" },
				NDButton = { &resources::Images::menuIconMFDND, L"ND" };

			MFDModeMenuViewButton
				autopilotButton { &resources::Images::menuIconMFDAutopilot, L"A/P", SettingsInterfaceMFDToolbarMode::autopilot },
				pressureButton { &resources::Images::menuIconMFDBaro, L"BARO", SettingsInterfaceMFDToolbarMode::baro },
				lightsButton { &resources::Images::menuIconMFDLights, L"Lights", SettingsInterfaceMFDToolbarMode::lights },
				trimButtonButton { &resources::Images::menuIconMFDTrim, L"Trim", SettingsInterfaceMFDToolbarMode::trim };

			std::array<MFDModeMenuViewButton*, 4> modeButtons {
				&autopilotButton,
				&pressureButton,
				&trimButtonButton,
				&lightsButton
			};

			const Route* getRoute() override;

		private:

	};
}