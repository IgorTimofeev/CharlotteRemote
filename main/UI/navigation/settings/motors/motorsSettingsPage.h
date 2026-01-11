#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/navigation/page.h"
#include "UI/elements/titler.h"

#include "types/generic.h"
#include "utils/string.h"

namespace pizda {
	using namespace YOBA;
	
	class MotorEditor : public Titler {
		public:
			MotorEditor(std::wstring_view title, MotorConfiguration* settings);
			
			void toSettings();
			
			Callback<> changed {};
			
		private:
			MotorConfiguration* _settings;
			
			Layout _mainLayout {};
			
			Rectangle _backgroundRect {};
			
			RelativeStackLayout _row {};
			
			TextField _min {};
			Titler _minTitle { L"Min", &_min };
			
			TextField _max {};
			Titler _maxTitle { L"Max", &_max };
			
			TextField _startup {};
			Titler _startupTitle { L"Startup", &_startup };
			
			TextField _offset {};
			Titler _offsetTitle { L"Offset", &_offset };
			
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
			
			void vaginoz(MotorEditor* motorEditor);
	};
}