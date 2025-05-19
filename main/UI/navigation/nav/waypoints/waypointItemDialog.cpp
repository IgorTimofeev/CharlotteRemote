#include "waypointItemDialog.h"

#include <rc.h>
#include <types/navigationData.h>
#include <UI/theme.h>

#include "waypointsPage.h"

namespace pizda {
	WaypointItemDialog::WaypointItemDialog(uint16_t waypointIndex) {
		auto& rc = RC::getInstance();
		auto& nd = rc.getNavigationData();

		const auto& waypointData = nd.waypoints[waypointIndex];

		title.setText(waypointData.name);

		// FPL origin
		Theme::applySecondary(&_FPLOriginButton);
		_FPLOriginButton.setVisible(waypointData.type == NavigationWaypointType::airport);
		_FPLOriginButton.setText(L"Set as origin");

		_FPLOriginButton.click += [&rc, &nd, waypointIndex, this] {
			rc.getApplication().scheduleTask([&rc, &nd, waypointIndex, this] {
				nd.flightPlan.origin = NavigationDataFlightPlanAirport(nd.getAirportIndex(waypointIndex), 0);

				hide();
				delete this;
			});
		};

		rows += &_FPLOriginButton;

		// FPL leg
		Theme::applySecondary(&_FPLLegButton);
		_FPLLegButton.setVisible(waypointData.type != NavigationWaypointType::airport);
		_FPLLegButton.setText(L"Add to FPL");

		_FPLLegButton.click += [&rc, &nd, waypointIndex, this] {
			rc.getApplication().scheduleTask([&rc, &nd, waypointIndex, this] {
				const auto it = std::ranges::find_if(nd.flightPlan.legs, [waypointIndex](const auto& leg) {
					return leg.waypointIndex == waypointIndex;
				});

				if (it != nd.flightPlan.legs.end())
					nd.flightPlan.legs.push_back(NavigationDataFlightPlanLeg(waypointIndex));

				hide();
				delete this;
			});
		};

		rows += &_FPLLegButton;

		// FPL destination
		Theme::applySecondary(&_FPLDestinationButton);
		_FPLDestinationButton.setVisible(waypointData.type == NavigationWaypointType::airport);
		_FPLDestinationButton.setText(L"Set as destination");

		_FPLDestinationButton.click += [&rc, &nd, waypointIndex, this] {
			rc.getApplication().scheduleTask([&rc, &nd, waypointIndex, this] {
				nd.flightPlan.destination = NavigationDataFlightPlanAirport(nd.getAirportIndex(waypointIndex), 0);

				hide();
				delete this;
			});
		};

		rows += &_FPLDestinationButton;

		// Remove button
		Theme::applyCritical(&_removeButton);
		_removeButton.setText(L"Delete");

		_removeButton.click += [&rc, &nd, waypointIndex, this] {
			rc.getApplication().scheduleTask([&rc, &nd, waypointIndex, this] {
				nd.removeWaypointAt(waypointIndex);

				const auto page = WaypointsPage::getInstance();

				if (page)
					page->updateFromNavigationData();

				hide();
				delete this;
			});
		};

		rows += &_removeButton;
	}
}