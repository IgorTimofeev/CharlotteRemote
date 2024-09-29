#pragma once

#include "../../../../lib/YOBA/src/hardware/screen/buffers/screenBuffer.h"
#include "../../../../lib/YOBA/src/ui/element.h"
#include "../../../../lib/YOBA/src/ui/shapes/rectangle.h"
#include "../../../../lib/YOBA/src/ui/stackLayout.h"
#include "../../../elements/titler.h"
#include "trimIndicator.h"
#include "throttleIndicator.h"
#include "controlsIndicator.h"

using namespace yoba;

namespace pizdanc {
	class InstrumentsLayout : public Layout {
		public:
			InstrumentsLayout();

			void tick();

		private:
			StackLayout _rows = StackLayout();

			Rectangle _backgroundRect = Rectangle(&Theme::bg1);

			ThrottleIndicator _throttle1Indicator = ThrottleIndicator();
			ThrottleIndicator _throttle2Indicator = ThrottleIndicator();

			StackLayout _throttleRow = StackLayout();
			Titler _throttleTitle = Titler(L"Eng", &_throttleRow);

			TrimIndicator _pitchTrimIndicator = TrimIndicator();
			Titler _pitchTitle = Titler(L"Trim", &_pitchTrimIndicator);

			ControlsIndicator _controlsIndicator = ControlsIndicator();
			Titler _controlsTitle = Titler(L"SPL/FLP", &_controlsIndicator);
	};
}