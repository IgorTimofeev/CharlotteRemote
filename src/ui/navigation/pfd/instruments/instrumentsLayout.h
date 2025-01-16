#pragma once

#include "../../../../lib/yoba/src/main.h"
#include "../../../../lib/yoba/src/ui.h"

#include "trimIndicator.h"
#include "throttleIndicator.h"
#include "controlsIndicator.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class InstrumentsLayout : public Layout {
		public:
			InstrumentsLayout();

			void tick() override;

		private:
			StackLayout _rows {};

			Rectangle _backgroundRect = Rectangle(&Theme::bg2);

			ThrottleIndicator _throttle1Indicator {};
			ThrottleIndicator _throttle2Indicator {};

			StackLayout _throttleRow {};
			Titler _throttleTitle = Titler(L"THR", &_throttleRow);

			ControlsIndicator _controlsIndicator {};
			Titler _controlsTitle = Titler(L"SP/FL", &_controlsIndicator);

			StackLayout _trimRow {};

			TrimIndicator _elevatorTrimIndicator {};
			Titler _elevatorTrimTitle = Titler(L"E", &_elevatorTrimIndicator);

			StackLayout _aileronsAndRudderRows {};

			TrimIndicator _aileronsTrimIndicator {};
			Titler _aileronsTrimTitle = Titler(L"A", &_aileronsTrimIndicator);

			TrimIndicator _rudderTrimIndicator {};
			Titler _rudderTrimTitle = Titler(L"R", &_rudderTrimIndicator);
	};
}