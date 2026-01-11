#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/titler.h"

#include "types/settings/settingsMotors.h"
#include "utils/string.h"

namespace pizda {
	using namespace YOBA;
	
	class MotorEditor : public Titler {
		public:
			MotorEditor(std::wstring_view title, SettingsMotor* settings);
			
			void toSettings();
			
		private:
			SettingsMotor* _settings;
			
			RelativeStackLayout _row {};
			TextField _min {};
			TextField _max {};
			TextField _startup {};
			TextField _offset {};
			Button _reverse {};
	};

	class MotorsSettingsPage : public ScrollViewPage {
		public:
			MotorsSettingsPage();
			
		private:
			MotorEditor throttle;
			MotorEditor noseWheel;
			MotorEditor aileronLeft;
			MotorEditor aileronRight;
			MotorEditor flapLeft;
			MotorEditor flapRight;
			MotorEditor tailLeft;
			MotorEditor tailRight;
			
			Button confirmButton {};
	};
}