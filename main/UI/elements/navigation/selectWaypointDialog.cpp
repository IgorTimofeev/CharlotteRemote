#include "selectWaypointDialog.h"

#include <utilities/string.h>

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	void SelectWaypointDialog::select(
		const std::wstring_view titleText,
		const std::function<void(const WaypointDialogSelectedItem& selectedItem)>& onConfirm
	) {
		(new SelectWaypointDialog(titleText, std::nullopt, onConfirm))->show();
	}

	void SelectWaypointDialog::edit(
		const std::wstring_view titleText,
		const WaypointDialogSelectedItem& selectedItem,
		const std::function<void(const WaypointDialogSelectedItem& selectedItem)>& onConfirm
	) {
		const auto& nd = RC::getInstance().getNavigationData();
		const auto& selectedWaypointData = nd.waypoints[selectedItem.waypointIndex];

		(new SelectWaypointDialog(
			titleText,
			selectedItem,
			onConfirm
		))->show();
	}

	SelectWaypointDialog::SelectWaypointDialog(
		const std::wstring_view titleText,
		const std::optional<WaypointDialogSelectedItem>& selectedItem,
		const std::function<void(const WaypointDialogSelectedItem& selectedItem)>& onConfirm
	) {
		auto& rc = RC::getInstance();

		title.setText(titleText);

		// Name
		Theme::apply(&_searchTextField);

		_searchTextField.setPlaceholder(L"Search");

		_searchTextField.setOnTextChanged([this, &rc] {
			if (!_searchTextField.isFocused())
				return;

			// Searching for waypoints by filter & name
			const auto it =
				_searchTextField.getText().length() == 0
				? rc.getNavigationData().waypoints.end()
				: std::ranges::find_if(
					rc.getNavigationData().waypoints,
					[this](const NavigationWaypointData& waypointData) {
						return StringUtils::containsIgnoreCase(waypointData.name, _searchTextField.getText());
					}
				);

			// Not found
			if (it == rc.getNavigationData().waypoints.end()) {
				_waypointTitle.setVisible(false);
			}
			// Found
			else {
				_waypointTitle.setVisible(true);

				const auto waypointIndex = std::distance(rc.getNavigationData().waypoints.begin(), it);
				const auto& waypointData = rc.getNavigationData().waypoints[waypointIndex];

				updateVisualsFromWaypoint(waypointIndex, waypointData);
			}
		});

		rows += &_searchTextFieldTitle;

		// Waypoint
		_waypointButton.setToggle(true);
		_waypointButton.setEnabled(false);
		rows += &_waypointTitle;

		// Confirm button
		Theme::applyPrimary(&_confirmButton);
		_confirmButton.setText(L"Confirm");

		_confirmButton.setOnClick([&rc, this, onConfirm] {
			rc.getApplication().scheduleOnTick([this, onConfirm] {
				if (_waypointButton.getWaypointIndex() < 0)
					return;

				onConfirm(WaypointDialogSelectedItem(_waypointButton.getWaypointIndex()));

				hide();
				delete this;
			});
		});

		rows += &_confirmButton;

		// Initialization
		if (selectedItem.has_value()) {
			_waypointTitle.setVisible(true);

			const auto& waypointData = rc.getNavigationData().waypoints[selectedItem.value().waypointIndex];

			_searchTextField.setText(waypointData.name);

			updateVisualsFromWaypoint(
				selectedItem.value().waypointIndex,
				waypointData
			);
		}
		else {
			_waypointTitle.setVisible(false);
		}
	}

	void SelectWaypointDialog::updateVisualsFromWaypoint(
		const uint16_t waypointIndex,
		const NavigationWaypointData& waypointData
	) {
		const auto& nd = RC::getInstance().getNavigationData();

		_waypointButton.setWaypointIndex(waypointIndex);

		// Runway
		if (waypointData.type == NavigationWaypointType::runway) {
			_waypointTitle.title.setText(L"Runway");

			// const auto runwayIndex = nd.getRunwayIndex(waypointIndex);
			// const auto& runway = nd.runways[runwayIndex];
		}
		// Waypoint
		else {
			_waypointTitle.title.setText(L"Waypoint");
		}
	}
}
