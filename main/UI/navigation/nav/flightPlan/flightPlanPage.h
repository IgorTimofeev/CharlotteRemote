#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../../page.h"
#include "../../../theme.h"

namespace pizda {
	using namespace YOBA;

	class FlightPlanButton : public Button {
		public:
			explicit FlightPlanButton(std::wstring_view text);
	};

	class FlightPlanPage : public ScrollViewPage {
		public:
			explicit FlightPlanPage();

		private:
			FlightPlanButton _buttons[5] {
				FlightPlanButton(L"LI746"),
				FlightPlanButton(L"OBARI"),
				FlightPlanButton(L"BIPRI"),
				FlightPlanButton(L"FF28L"),
				FlightPlanButton(L"ULLI28L"),
			};
	};
}