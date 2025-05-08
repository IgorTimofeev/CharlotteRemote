#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "menuView.h"
#include "menuViewButton.h"
#include "../../../resources/images.h"

namespace pizda {
	using namespace YOBA;

	class MFDModeMenuViewButton : public MenuViewButton {
		public:
			explicit MFDModeMenuViewButton(const Image* image, std::wstring_view text, SettingsInterfaceMFDInstrumentsMode mode);

		protected:
			void onClick() override;

		private:
			SettingsInterfaceMFDInstrumentsMode _mode;
	};

	class MFDMenuView : public MenuView {
		public:
			explicit MFDMenuView();

			MenuViewButton
				NDButton = MenuViewButton(&resources::Images::menuIconND, L"ND");

			MFDModeMenuViewButton
				mainButton = MFDModeMenuViewButton(&resources::Images::menuIconMFDMain, L"ECAM", SettingsInterfaceMFDInstrumentsMode::main),
				autopilotButton = MFDModeMenuViewButton(&resources::Images::menuIconMFDAutopilot, L"MCP", SettingsInterfaceMFDInstrumentsMode::autopilot),
				pressureButton = MFDModeMenuViewButton(&resources::Images::menuIconMFDPressure, L"EFIS", SettingsInterfaceMFDInstrumentsMode::pressure);

			MFDModeMenuViewButton* modeButtons[3] {
				&mainButton,
				&autopilotButton,
				&pressureButton
			};

			const Route* getRoute() override;
	};
}