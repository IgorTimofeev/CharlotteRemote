#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/menu/pagesMenuView.h"
#include "UI/navigation/menu/pageMenuViewButton.h"
#include "UI/navigation/routes.h"
#include "resources/images.h"

namespace pizda {
	using namespace YOBA;

	class SettingsMenuView : public PagesMenuView {
		public:
			void setup() override;
		
		private:
			static const Route* _lastRoute;
			
			std::array<PageMenuViewButton, 7> _buttons {
				PageMenuViewButton(&resources::images::menuIconPersonalization, "Personal", &Routes::settingsPersonalization),
				PageMenuViewButton(&resources::images::menuIconADIRS, "ADIRS", &Routes::settingsADIRS),
				PageMenuViewButton(&resources::images::menuIconSpectrumScan, "Scan", &Routes::settingsSpectrumScan),
				PageMenuViewButton(&resources::images::menuIconTransceiverImage, "XCVR", &Routes::settingsTransceiver),
				PageMenuViewButton(&resources::images::menuIconAxes, "Axes", &Routes::settingsAxes),
				PageMenuViewButton(&resources::images::menuIconMotors, "Motors", &Routes::settingsMotors),
				PageMenuViewButton(&resources::images::menuIconAutopilotSettings, "Autopilot", &Routes::settingsAutopilot),
			};
	};
}