#include "airportFlightPlanItemDialog.h"

#include <rc.h>
#include <types/navigationData.h>
#include <UI/theme.h>

#include "flightPlanPage.h"

namespace pizda {
	AirportFlightPlanItemDialog::AirportFlightPlanItemDialog(const NavigationDataFlightPlanAirport& flightPlanAirport, bool destination) {
		auto& rc = RC::getInstance();
		auto& nd = rc.getNavigationData();

		const auto& airport = nd.airports[flightPlanAirport.airportIndex];
		const auto& waypointData = nd.waypoints[airport.waypointIndex];

		title.setText(waypointData.name);

		// Remove button
		Theme::applyCritical(&_removeButton);
		_removeButton.setText(L"Delete");

		_removeButton.click += [&rc, &nd, this, destination] {
			rc.getApplication().scheduleTask([&rc, &nd, this, destination] {
				if (destination) {
					nd.flightPlan.destination = std::nullopt;
				}
				else {
					nd.flightPlan.origin = std::nullopt;
				}

				const auto page = FlightPlanPage::getInstance();

				if (page)
					page->updateFromNavigationData();

				hide();
				delete this;
			});
		};

		rows += &_removeButton;
	}
}