#include "waypointItemDialog.h"

#include <rc.h>
#include <types/navigationData.h>
#include <UI/theme.h>

#include "waypointsPage.h"

namespace pizda {
	void WaypointItemDialog::showCyka(uint16_t waypointIndex) {
		const auto dialog = new WaypointItemDialog(waypointIndex);
		dialog->show();
	}

	WaypointItemDialog::WaypointItemDialog(uint16_t waypointIndex) {
		auto& rc = RC::getInstance();
		auto& nd = rc.getNavigationData();

		const auto& waypointData = nd.waypoints[waypointIndex];

		title.setText(waypointData.name);

		// FPL origin
		Theme::applySecondary(&FPLOriginButton);
		FPLOriginButton.setVisible(waypointData.type == NavigationWaypointType::airport);
		FPLOriginButton.setText(L"Set as origin");

		FPLOriginButton.click += [&rc, &nd, waypointIndex, this] {
			rc.getApplication().scheduleTask([&rc, &nd, waypointIndex, this] {
				nd.flightPlan.origin = NavigationDataFlightPlanAirport(nd.getAirportIndex(waypointIndex), 0);

				hide();
				delete this;
			});
		};

		rows += &FPLOriginButton;

		// FPL leg
		Theme::applySecondary(&FPLLegButton);
		FPLLegButton.setVisible(waypointData.type != NavigationWaypointType::airport);
		FPLLegButton.setText(L"Add to FPL");

		FPLLegButton.click += [&rc, &nd, waypointIndex, this] {
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

		rows += &FPLLegButton;

		// FPL destination
		Theme::applySecondary(&FPLDestinationButton);
		FPLDestinationButton.setVisible(waypointData.type == NavigationWaypointType::airport);
		FPLDestinationButton.setText(L"Set as destination");

		FPLDestinationButton.click += [&rc, &nd, waypointIndex, this] {
			rc.getApplication().scheduleTask([&rc, &nd, waypointIndex, this] {
				nd.flightPlan.destination = NavigationDataFlightPlanAirport(nd.getAirportIndex(waypointIndex), 0);

				hide();
				delete this;
			});
		};

		rows += &FPLDestinationButton;

		// Remove button
		Theme::applyCritical(&removeButton);
		removeButton.setText(L"Delete");

		removeButton.click += [&rc, &nd, waypointIndex, this] {
			rc.getApplication().scheduleTask([&rc, &nd, waypointIndex, this] {
				nd.removeWaypointAt(waypointIndex);

				const auto page = WaypointsPage::getInstance();

				if (page)
					page->updateFromNavigationData();

				hide();
				delete this;
			});
		};

		rows += &removeButton;
	}
}