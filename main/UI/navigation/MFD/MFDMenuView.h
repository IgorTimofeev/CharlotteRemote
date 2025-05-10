#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../menu/menuView.h"
#include "../menu/menuViewButton.h"
#include "../../../resources/images.h"

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
				mainButton { &resources::Images::menuIconMFDMain, L"ECAM", SettingsInterfaceMFDToolbarMode::main },
				autopilotButton { &resources::Images::menuIconMFDAutopilot, L"MCP", SettingsInterfaceMFDToolbarMode::autopilot },
				pressureButton { &resources::Images::menuIconMFDPressure, L"EFIS", SettingsInterfaceMFDToolbarMode::pressure };

			MFDModeMenuViewButton* modeButtons[3] {
				&mainButton,
				&autopilotButton,
				&pressureButton
			};

			const Route* getRoute() override;

		private:

	};
}