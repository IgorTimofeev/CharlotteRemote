#include "flightPlanPage.h"

#include "../../../../rc.h"

#include <numbers>
#include <format>
#include <esp_log.h>

namespace pizda {
	FlightPlanButton::FlightPlanButton(std::wstring_view text) {
		setHeight(24);

		setCornerRadius(2);

		setDefaultBackgroundColor(&Theme::bg2);
		setDefaultTextColor(&Theme::purple);

		setActiveBackgroundColor(&Theme::fg1);
		setActiveTextColor(&Theme::bg1);

		setFont(&Theme::fontSmall);
		setText(text);
	}

	FlightPlanPage::FlightPlanPage() {
		title.setText(L"Flight plan");

		rows.setMargin(Margin(8));
		rows.setSpacing(5);

		for (auto& button : _buttons) {
			rows += &button;
		}
	}
}
