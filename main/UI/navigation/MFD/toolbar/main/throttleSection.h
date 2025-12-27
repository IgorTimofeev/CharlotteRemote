#pragma once

#include "UI/navigation/MFD/toolbar/toolbarSection.h"
#include "throttleIndicator.h"

namespace pizda {
	using namespace YOBA;

	class ThrottleSection : public ToolbarSection {
		public:
			explicit ThrottleSection();

			StackLayout row {};
			ThrottleIndicator throttleIndicator1 {};
			ThrottleIndicator throttleIndicator2 {};

		protected:
			void onEventBeforeChildren(Event* event) override;
			void onTick() override;
	};
}