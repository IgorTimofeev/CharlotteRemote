#include "flightPlanItemDialog.h"

#include "rc.h"
#include "types/navigationData.h"
#include "UI/theme.h"
#include "UI/elements/navigation/selectWaypointDialog.h"

#include "flightPlanPage.h"

namespace pizda {
	FlightPlanItemDialog::FlightPlanItemDialog(uint16_t legIndex) {
		auto& rc = RC::getInstance();

		const auto& leg = rc.getNavigationData().flightPlan.legs[legIndex];
		const auto& waypointData = rc.getNavigationData().waypoints[leg.waypointIndex];

		title.setText(waypointData.name);

		// Activate leg
		Theme::applyPrimary(&_activateLegButton);
		_activateLegButton.setText(L"Activate leg");

		_activateLegButton.setOnClick([this, &rc, legIndex] {
			rc.getApplication().invokeOnNextTick([this, &rc, legIndex] {
				rc.getNavigationData().flightPlan.activeLegIndex = legIndex;

				const auto page = FlightPlanPage::getInstance();

				if (page)
					page->updateFromNavigationData();

				hide();
				delete this;
			});
		});

		rows += &_activateLegButton;

		// Edit
		Theme::applySecondary(&_editButton);
		_editButton.setText(L"Replace");

		_editButton.setOnClick([this, &waypointData, legIndex, &leg, &rc] {
			rc.getApplication().invokeOnNextTick([&rc, this, &leg, legIndex, &waypointData] {
				SelectWaypointDialog::edit(
					std::format(L"Change {}", waypointData.name),
					WaypointDialogSelectedItem(leg.waypointIndex),
					[legIndex, &rc](const WaypointDialogSelectedItem& selectedItem) {
						rc.getNavigationData().flightPlan.legs[legIndex] = NavigationDataFlightPlanLeg(selectedItem.waypointIndex);

						const auto page = FlightPlanPage::getInstance();

						if (page)
							page->updateFromNavigationData();
					}
				);

				hide();
				delete this;
			});
		});

		rows += &_editButton;

		// Insert before
		Theme::applySecondary(&_insertBeforeButton);
		_insertBeforeButton.setText(L"Insert before");

		_insertBeforeButton.setOnClick([this, &waypointData, legIndex] {
			insertOnButtonClick(
				std::format(L"Insert before {}", waypointData.name),
				legIndex
			);
		});

		rows += &_insertBeforeButton;

		// Insert after
		Theme::applySecondary(&_insertAfterButton);
		_insertAfterButton.setText(L"Insert after");

		_insertAfterButton.setOnClick([this, &waypointData, legIndex] {
			insertOnButtonClick(
				std::format(L"Insert after {}", waypointData.name),
				legIndex + 1
			);
		});

		rows += &_insertAfterButton;

		// Remove
		Theme::applyCritical(&_removeButton);
		_removeButton.setText(L"Delete");

		_removeButton.setOnClick([&rc, this, legIndex] {
			rc.getApplication().invokeOnNextTick([&rc, this, legIndex] {
				rc.getNavigationData().flightPlan.legs.erase(rc.getNavigationData().flightPlan.legs.begin() + legIndex);

				const auto page = FlightPlanPage::getInstance();

				if (page)
					page->updateFromNavigationData();

				hide();
				delete this;
			});
		});

		rows += &_removeButton;
	}

	void FlightPlanItemDialog::showWaypointSelectionDialogToInsertAt(const std::wstring_view title, size_t insertAt) {
		SelectWaypointDialog::select(
			title,
			[insertAt](const WaypointDialogSelectedItem& selectedItem) {
				auto& rc = RC::getInstance();

				rc.getNavigationData().flightPlan.legs.insert(
					rc.getNavigationData().flightPlan.legs.begin() + insertAt,
					NavigationDataFlightPlanLeg(selectedItem.waypointIndex)
				);

				const auto page = FlightPlanPage::getInstance();

				if (page)
					page->updateFromNavigationData();
			}
		);
	}

	void FlightPlanItemDialog::insertOnButtonClick(const std::wstring& title, uint16_t insertAt) {
		RC::getInstance().getApplication().invokeOnNextTick([this, insertAt, title] {
			showWaypointSelectionDialogToInsertAt(title, insertAt);

			hide();
			delete this;
		});
	}
}