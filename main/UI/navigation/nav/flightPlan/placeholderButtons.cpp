#include "placeholderButtons.h"

#include <esp_log.h>
#include <UI/theme.h>
#include <UI/elements/navigation/selectWaypointDialog.h>
#include <UI/elements/navigation/waypointButton.h>
#include <types/navigationData.h>
#include <rc.h>

#include "flightPlanPage.h"
#include "legFlightPlanItemDialog.h"

namespace pizda {
	AirportPlaceholderButton::AirportPlaceholderButton(bool destination): _destination(destination) {
		Theme::applyPlaceholder(this);
		setHeight(WaypointButton::height);
		setText(destination ? L"Add destination" : L"Add origin");
	}

	void AirportPlaceholderButton::onClick() {
		Button::onClick();

		SelectWaypointDialog::select(
			getText(),
			true,
			[this](const WaypointDialogSelectedItem& selectedItem) {
				auto& nd = RC::getInstance().getNavigationData();

				const auto flightPlanAirport = NavigationDataFlightPlanAirport(selectedItem.airportAndRunway.value());

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
		);
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
