#pragma once

#include "../../../../lib/YOBA/src/hardware/screen/buffers/screenBuffer.h"
#include "../../../../lib/YOBA/src/ui/element.h"
#include "../../../../lib/YOBA/src/ui/rectangle.h"
#include "../../../../lib/YOBA/src/ui/stackContainer.h"
#include "../../../elements/titler.h"
#include "trimIndicator.h"
#include "throttleIndicator.h"
#include "controlsIndicator.h"

using namespace yoba;

namespace pizdanc {
	class InstrumentsLayout : public Container {
		public:
			InstrumentsLayout();

			void setup();

		private:
			StackContainer _rows {};

			Rectangle _backgroundRect = Rectangle(&Theme::bg1);

			ThrottleIndicator _throttle1Indicator {};
			ThrottleIndicator _throttle2Indicator {};

			StackContainer _throttleRow {};
			Titler _throttleTitle = Titler(U"THR", &_throttleRow);

			ControlsIndicator _controlsIndicator {};
			Titler _controlsTitle = Titler(U"SP/FU", &_controlsIndicator);

			StackContainer _trimRow {};

			TrimIndicator _elevatorTrimIndicator {};
			Titler _elevatorTrimTitle = Titler(U"E", &_elevatorTrimIndicator);

			StackContainer _aileronsAndRudderRows {};

			TrimIndicator _aileronsTrimIndicator {};
			Titler _aileronsTrimTitle = Titler(U"A", &_aileronsTrimIndicator);

			TrimIndicator _rudderTrimIndicator {};
			Titler _rudderTrimTitle = Titler(U"R", &_rudderTrimIndicator);
	};
}