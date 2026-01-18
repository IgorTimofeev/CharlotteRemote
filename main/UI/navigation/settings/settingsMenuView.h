#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/menu/pagesMenuView.h"
#include "UI/navigation/menu/pageMenuViewButton.h"
#include "UI/navigation/routes.h"
#include <resources/images.h>

namespace pizda {
	using namespace YOBA;

	class SettingsMenuView : public PagesMenuView {
		public:
			void setup() override;
		
		private:
			static const Route* _lastRoute;
			
			std::array<PageMenuViewButton, 5> _buttons {
				PageMenuViewButton(&resources::images::menuIconPersonalization, L"Personal", &Routes::settingsPersonalization),
				PageMenuViewButton(&resources::images::menuIconCalibration, L"Calibrate", &Routes::settingsCalibration),
				PageMenuViewButton(&resources::images::menuIconAxis, L"Axis", &Routes::settingsAxis),
				PageMenuViewButton(&resources::images::menuIconMotors, L"Motors", &Routes::settingsMotors),
				PageMenuViewButton(&resources::images::menuIconDev, L"Dev", &Routes::settingsDeveloper)
			};
	};
}