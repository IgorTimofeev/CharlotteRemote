#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"

#include "menuView.h"
#include "menuButton.h"
#include "../../../settings.h"
#include "../../../resources/images.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

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
				NDButton = MenuButton(&resources::Images::menuIconMFDND, L"N/D");

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