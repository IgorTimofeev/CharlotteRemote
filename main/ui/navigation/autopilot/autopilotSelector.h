#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class AutopilotSelector : public Layout {
		public:
			AutopilotSelector(uint8_t digitCount, std::wstring_view titleText, std::wstring_view buttonText);

			Rectangle background;
			EqualLayout row = EqualLayout(Orientation::horizontal);
			SevenSegment seven;
			Knob knob;
			Button button;
			Titler titler;

		private:
	};
}