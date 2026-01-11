#include "airportFlightPlanItemDialog.h"

#include "rc.h"
#include "types/navigationData.h"
#include "UI/theme.h"
#include "UI/elements/navigation/selectWaypointDialog.h"

#include "flightPlanPage.h"

namespace pizda {
	AirportFlightPlanItemDialog::AirportFlightPlanItemDialog(const NavigationAirportAndRunwayIndicesData& airportAndRunway, bool destination) {
		auto& rc = RC::getInstance();
		auto& nd = rc.getNavigationData();

		const auto& airport = nd.airports[airportAndRunway.airportIndex];
		const auto& waypointData = nd.waypoints[airport.waypointIndex];

		title.setText(std::format(L"{} - {}", waypointData.name, destination ? L"destination" : L"origin"));

		// Edit
		Theme::applySecondary(&_editButton);
		_editButton.setText(L"Change");

		_editButton.click += [this, &waypointData, &airport, &airportAndRunway, destination, &nd, &rc] {
			rc.getApplication().scheduleOnTick([&rc, &nd, this, destination, &waypointData, &airport, &airportAndRunway] {
				SelectWaypointDialog::edit(
					std::format(L"Change {}", waypointData.name),
					WaypointDialogSelectedItem(airport.waypointIndex, airportAndRunway),
					[this, destination, &nd](const WaypointDialogSelectedItem& selectedItem) {
						const auto flightPlanAirport = NavigationDataFlightPlanAirport(selectedItem.airportAndRunway.value());

						if (destination) {
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

				hide();
				delete this;
			});
		};

		rows += &_editButton;

		// Remove button
		Theme::applyCritical(&_removeButton);
		_removeButton.setText(L"Delete");

		_removeButton.click += [&rc, &nd, this, destination] {
			rc.getApplication().scheduleOnTick([&rc, &nd, this, destination] {
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