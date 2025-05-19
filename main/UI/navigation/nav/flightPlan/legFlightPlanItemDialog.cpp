#include "legFlightPlanItemDialog.h"

#include <rc.h>
#include <types/navigationData.h>
#include <UI/theme.h>
#include <UI/elements/navigation/selectWaypointDialog.h>

#include "flightPlanPage.h"

namespace pizda {
	LegFlightPlanItemDialog::LegFlightPlanItemDialog(uint16_t legIndex) {
		auto& rc = RC::getInstance();
		auto& nd = rc.getNavigationData();

		const auto& leg = nd.flightPlan.legs[legIndex];
		const auto& waypointData = nd.waypoints[leg.waypointIndex];

		title.setText(waypointData.name);

		// Insert before
		Theme::applySecondary(&_insertBeforeButton);
		_insertBeforeButton.setText(L"Insert before");

		_insertBeforeButton.click += [this, &waypointData, legIndex] {
			insertWaypoint(
				std::format(L"Insert before {}", waypointData.name),
				legIndex
			);
		};

		rows += &_insertBeforeButton;

		// Insert after
		Theme::applySecondary(&_insertAfterButton);
		_insertAfterButton.setText(L"Insert after");

		_insertAfterButton.click += [this, &waypointData, legIndex] {
			insertWaypoint(
				std::format(L"Insert after {}", waypointData.name),
				legIndex + 1
			);
		};

		rows += &_insertAfterButton;

		// Remove
		Theme::applyCritical(&_removeButton);
		_removeButton.setText(L"Delete");

		_removeButton.click += [&rc, &nd, this, &leg, legIndex] {
			rc.getApplication().scheduleOnTick([&rc, &nd, this, &leg, legIndex] {
				nd.flightPlan.legs.erase(nd.flightPlan.legs.begin() + legIndex);

				const auto page = FlightPlanPage::getInstance();

				if (page)
					page->updateFromNavigationData();

				hide();
				delete this;
			});
		};

		rows += &_removeButton;
	}

	void LegFlightPlanItemDialog::showWaypointSelectionDialogToInsertAt(std::wstring_view title, size_t insertAt) {
		(new SelectWaypointDialog(
			title,
			[](const NavigationWaypointData& waypointData) {
				return waypointData.type != NavigationWaypointType::airport;
			},
			[insertAt](uint16_t waypointIndex, const std::optional<NavigationAirportIndexAndRunwayIndexData>&) {
				auto& rc = RC::getInstance();
				auto& nd = rc.getNavigationData();

				nd.flightPlan.legs.insert(nd.flightPlan.legs.begin() + insertAt, NavigationDataFlightPlanLeg(waypointIndex));

				const auto page = FlightPlanPage::getInstance();

				if (page)
					page->updateFromNavigationData();
			}
		))->show();
	}

	void LegFlightPlanItemDialog::insertWaypoint(const std::wstring& title, uint16_t insertAt) {
		RC::getInstance().getApplication().scheduleOnTick([this, insertAt, title] {
			hide();
			delete this;

			showWaypointSelectionDialogToInsertAt(title, insertAt);
		});
	}
}