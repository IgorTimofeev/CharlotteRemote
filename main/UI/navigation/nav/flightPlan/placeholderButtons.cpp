#include "placeholderButtons.h"

#include <esp_log.h>
#include <UI/theme.h>
#include <UI/elements/navigation/selectWaypointDialog.h>
#include <UI/elements/navigation/waypointButton.h>
#include <types/navigationData.h>
#include <rc.h>

#include "legFlightPlanItemDialog.h"

namespace pizda {
	AirportPlaceholderButton::AirportPlaceholderButton(bool destination): _destination(destination) {
		Theme::applyPlaceholder(this);
		setHeight(WaypointButton::height);
		setText(destination ? L"Add destination" : L"Add origin");
	}

	void AirportPlaceholderButton::onClick() {
		Button::onClick();

		(new SelectWaypointDialog(
			getText(),
			[](const NavigationWaypointData& waypointData) {
				return waypointData.type == NavigationWaypointType::airport;
			},
			[this](uint16_t, const std::optional<NavigationAirportIndexAndRunwayIndexData>& airportAndRunway) {
				auto& nd = RC::getInstance().getNavigationData();

				const auto flightPlanAirport = NavigationDataFlightPlanAirport(airportAndRunway.value().airportIndex, airportAndRunway.value().runwayIndex);

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
		Theme::applyPlaceholder(this);
		setHeight(WaypointButton::height);
		setText(L"Add route");
	}

	void LegPlaceholderButton::onClick() {
		Button::onClick();

		LegFlightPlanItemDialog::showWaypointSelectionDialogToInsertAt(getText(), 0);
	}
}
