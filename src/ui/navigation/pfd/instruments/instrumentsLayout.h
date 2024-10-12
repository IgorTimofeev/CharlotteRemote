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

			void setup();

		private:
			StackLayout _rows = StackLayout();

			Rectangle _backgroundRect = Rectangle(&Theme::bg1);

			ThrottleIndicator _throttle1Indicator = ThrottleIndicator();
			ThrottleIndicator _throttle2Indicator = ThrottleIndicator();

			StackLayout _throttleRow = StackLayout();
			Titler _throttleTitle = Titler(L"THR", &_throttleRow);

			ControlsIndicator _controlsIndicator = ControlsIndicator();
			Titler _controlsTitle = Titler(L"SP/FL", &_controlsIndicator);

			StackLayout _trimRow = StackLayout();

			TrimIndicator _elevatorTrimIndicator = TrimIndicator();
			Titler _elevatorTrimTitle = Titler(L"E", &_elevatorTrimIndicator);

			StackLayout _aileronsAndRudderRows = StackLayout();

			TrimIndicator _aileronsTrimIndicator = TrimIndicator();
			Titler _aileronsTrimTitle = Titler(L"A", &_aileronsTrimIndicator);

			TrimIndicator _rudderTrimIndicator = TrimIndicator();
			Titler _rudderTrimTitle = Titler(L"R", &_rudderTrimIndicator);
	};
}