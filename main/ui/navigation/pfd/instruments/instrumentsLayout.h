#pragma once

#include "../../../../../components/yoba/src/main.h"
#include "../../../../../components/yoba/src/ui.h"

#include "trimIndicator.h"
#include "throttleIndicator.h"
#include "controlsIndicator.h"

namespace pizdanc {
	using namespace yoba;
	using namespace yoba::ui;

	class InstrumentsLayout : public Layout {
		public:
			InstrumentsLayout();

			static const uint8_t panelSize = 52;

		protected:
			void onTick() override;

		private:
			StackLayout _row;

			Rectangle _backgroundRect;
			Rectangle _titlesRect;

			ThrottleIndicator _throttle1Indicator;
			ThrottleIndicator _throttle2Indicator;

			StackLayout _throttleRow;
			Titler _throttleTitle = Titler(L"THR", &_throttleRow);

			ControlsIndicator _controlsIndicator;
			Titler _controlsTitle = Titler(L"SP/FL", &_controlsIndicator);

			TrimIndicator _elevatorTrimIndicator;
			Titler _elevatorTrimTitle = Titler(L"Trim", &_elevatorTrimIndicator);
	};
}