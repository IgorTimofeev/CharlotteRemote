#include "placeholderButton.h"

#include <rc.h>

#include "../../../elements/navigation/selectWaypointDialog.h"

namespace pizda {
	PlaceholderButton::PlaceholderButton() {
		setHeight(32);
		setCornerRadius(3);

		setDefaultBorderColor(&Theme::fg5);
		setDefaultTextColor(&Theme::fg5);

		setActiveBorderColor(&Theme::fg1);
		setActiveTextColor(&Theme::fg1);

		setFont(&Theme::fontNormal);
	}

	AirportPlaceholderButton::AirportPlaceholderButton(bool destination): _destination(destination) {
		setText(destination ? L"Add destination" : L"Add origin");
	}

	void AirportPlaceholderButton::onClick() {
		Button::onClick();

		(new SelectWaypointDialog(
			getText(),
			[](const NavigationWaypointData& waypointData) {
				return waypointData.type == NavigationWaypointType::airport;
			},
			[this](uint16_t waypointIndex) {
				auto& rc = RC::getInstance();
				auto& nd = rc.getNavigationData();

				const auto flightPlanAirport = NavigationDataFlightPlanAirport(nd.getAirportIndex(waypointIndex), 0);

				if (_destination) {
					nd.flightPlan.destination = flightPlanAirport;
				}
				else {
					nd.flightPlan.origin = flightPlanAirport;
				}

				const auto page = FlightPlanPage::getInstance();

				if (page)
					page->updateFromNavigationData();
			}
		))->show();
	}

	LegPlaceholderButton::LegPlaceholderButton() {
		setText(L"Add route");
	}

	void LegPlaceholderButton::onClick() {
		Button::onClick();

		(new SelectWaypointDialog(
			getText(),
			[](const NavigationWaypointData& waypointData) {
				return waypointData.type != NavigationWaypointType::airport;
			},
			[](uint16_t waypointIndex) {
				auto& rc = RC::getInstance();
				auto& nd = rc.getNavigationData();

				nd.flightPlan.legs.push_back(NavigationDataFlightPlanLeg(waypointIndex));

				const auto page = FlightPlanPage::getInstance();

				if (page)
					page->updateFromNavigationData();
			}
		))->show();
	}
}
