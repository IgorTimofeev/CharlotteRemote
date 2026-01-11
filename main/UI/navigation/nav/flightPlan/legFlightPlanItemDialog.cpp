#include "legFlightPlanItemDialog.h"

#include "rc.h"
#include "types/navigationData.h"
#include "UI/theme.h"
#include "UI/elements/navigation/selectWaypointDialog.h"

#include "flightPlanPage.h"

namespace pizda {
	LegFlightPlanItemDialog::LegFlightPlanItemDialog(uint16_t legIndex) {
		auto& rc = RC::getInstance();
		auto& nd = rc.getNavigationData();

		const auto& leg = nd.flightPlan.legs[legIndex];
		const auto& waypointData = nd.waypoints[leg.waypointIndex];

		title.setText(waypointData.name);

		// Edit
		Theme::applySecondary(&_editButton);
		_editButton.setText(L"Change");

		_editButton.click += [this, &waypointData, legIndex, &leg, &rc] {
			rc.getApplication().scheduleOnTick([&rc, this, &leg, legIndex, &waypointData] {
				SelectWaypointDialog::edit(
					std::format(L"Change {}", waypointData.name),
					WaypointDialogSelectedItem(leg.waypointIndex, std::nullopt),
					[legIndex](const WaypointDialogSelectedItem& selectedItem) {
						auto& rc = RC::getInstance();
						auto& nd = rc.getNavigationData();

						nd.flightPlan.legs[legIndex] = NavigationDataFlightPlanLeg(selectedItem.waypointIndex);

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

		// Insert before
		Theme::applySecondary(&_insertBeforeButton);
		_insertBeforeButton.setText(L"Insert before");

		_insertBeforeButton.click += [this, &waypointData, legIndex] {
			insertOnButtonClick(
				std::format(L"Insert before {}", waypointData.name),
				legIndex
			);
		};

		rows += &_insertBeforeButton;

		// Insert after
		Theme::applySecondary(&_insertAfterButton);
		_insertAfterButton.setText(L"Insert after");

		_insertAfterButton.click += [this, &waypointData, legIndex] {
			insertOnButtonClick(
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
		SelectWaypointDialog::select(
			title,
			false,
			[insertAt](const WaypointDialogSelectedItem& selectedItem) {
				auto& rc = RC::getInstance();
				auto& nd = rc.getNavigationData();

				nd.flightPlan.legs.insert(nd.flightPlan.legs.begin() + insertAt, NavigationDataFlightPlanLeg(selectedItem.waypointIndex));

				const auto page = FlightPlanPage::getInstance();

				if (page)
					page->updateFromNavigationData();
			}
		);
	}

	void LegFlightPlanItemDialog::insertOnButtonClick(const std::wstring& title, uint16_t insertAt) {
		RC::getInstance().getApplication().scheduleOnTick([this, insertAt, title] {
			showWaypointSelectionDialogToInsertAt(title, insertAt);

			hide();
			delete this;
		});
	}
}