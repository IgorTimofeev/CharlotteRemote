#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"

#include "menuView.h"
#include "menuButton.h"
#include "../../../settings.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class MFDModeMenuButton : public MenuButton {
		public:
			explicit MFDModeMenuButton(const std::wstring_view& text, SettingsInterfaceMFDInstrumentsMode mode);

			void updateSelection();

		protected:
			void onClick() override;

		private:
			SettingsInterfaceMFDInstrumentsMode _mode;
	};

	class MFDMenuView : public MenuView {
		public:
			explicit MFDMenuView();

			OptionMenuButton
				NDButton = OptionMenuButton(L"N/D");

			MFDModeMenuButton
				mainButton = MFDModeMenuButton(L"Main", SettingsInterfaceMFDInstrumentsMode::main),
				autopilotButton = MFDModeMenuButton(L"A/P", SettingsInterfaceMFDInstrumentsMode::autopilot),
				pressureButton = MFDModeMenuButton(L"Baro", SettingsInterfaceMFDInstrumentsMode::pressure);

			MFDModeMenuButton* modeButtons[3] {
				&mainButton,
				&autopilotButton,
				&pressureButton
			};

			ViewMenuButton mainViewButton;
	};
}