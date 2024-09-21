#pragma once

#include "Arduino.h"

#include "../../../../lib/YOBA/src/ui/text.h"
#include "../../../../lib/YOBA/src/ui/slider.h"
#include "../../../../lib/YOBA/src/ui/sevenSegment.h"
#include "../../../../lib/YOBA/src/ui/rotaryKnob.h"

#include "../page.h"
#include "../../elements/titler.h"
#include "../../elements/rows.h"
#include "autopilotSelector.h"

using namespace yoba;

namespace pizdanc {
	class RCApplication;

	class AutopilotPage : public Page {
		public:
			StackLayout columns = StackLayout();

			void begin() override;

		private:
			AutopilotSelector spd = AutopilotSelector(L"Speed");
			AutopilotSelector alt = AutopilotSelector(L"Alt");
			AutopilotSelector pressure = AutopilotSelector(L"Baro");;

			static void addIndicatorCallback(AutopilotSelector& selector, float defaultValue, const std::function<void(float)>& callback);
	};
}