#pragma once

#include <UI/navigation/MFD/toolbar/instrumentIndicatorLayout.h>
#include "throttleIndicator.h"

namespace pizda {
	using namespace YOBA;

	class ThrottleIndicatorLayout : public InstrumentIndicatorLayout {
		public:
			explicit ThrottleIndicatorLayout();

			StackLayout row {};
			ThrottleIndicator throttleIndicator1 {};
			ThrottleIndicator throttleIndicator2 {};

		protected:
			void onEvent(Event* event) override;
			void onTick() override;
	};
}