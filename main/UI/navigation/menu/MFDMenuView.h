#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "menuView.h"
#include "menuButton.h"
#include "../../../settings.h"
#include "../../../resources/images.h"

namespace pizda {
	using namespace YOBA;

	class MFDModeMenuButton : public MenuButton {
		public:
			explicit MFDModeMenuButton(const Image* image, const std::wstring_view& text, SettingsInterfaceMFDInstrumentsMode mode);

		protected:
			void onClick() override;

		private:
			SettingsInterfaceMFDInstrumentsMode _mode;
	};

	class MFDMenuView : public MenuView {
		public:
			explicit MFDMenuView();

			MenuButton
				NDButton = MenuButton(&resources::Images::menuIconND, L"N/D");

			MFDModeMenuButton
				mainButton = MFDModeMenuButton(&resources::Images::menuIconMFDMain, L"Main", SettingsInterfaceMFDInstrumentsMode::main),
				autopilotButton = MFDModeMenuButton(&resources::Images::menuIconMFDAutopilot, L"A/P", SettingsInterfaceMFDInstrumentsMode::autopilot),
				pressureButton = MFDModeMenuButton(&resources::Images::menuIconMFDPressure, L"Baro", SettingsInterfaceMFDInstrumentsMode::pressure);

			MFDModeMenuButton* modeButtons[3] {
				&mainButton,
				&autopilotButton,
				&pressureButton
			};

			ViewMenuButton mainViewButton;
	};
}