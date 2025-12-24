#include "selectWaypointDialog.h"

#include "rc.h"
#include <UI/theme.h>
#include <utils/string.h>
#include <esp_log.h>
#include <esp_log.h>

namespace pizda {
	RunwayItem::RunwayItem(const NavigationAirportAndRunwayIndicesData& airportAndRunway) : airportAndRunway(airportAndRunway) {
		const auto& nd = RC::getInstance().getNavigationData();
		setText(nd.airports[airportAndRunway.airportIndex].runways[airportAndRunway.runwayIndex].getFormattedName());
	}

	WaypointDialogSelectedItem::WaypointDialogSelectedItem(uint16_t waypointIndex,
		const std::optional<NavigationAirportAndRunwayIndicesData>& selectedItem):
		NavigationWaypointDataIndexAware(waypointIndex),
		airportAndRunway(selectedItem) {

	}

	void SelectWaypointDialog::select(std::wstring_view titleText, bool airportsOnly,
		const std::function<void(const WaypointDialogSelectedItem& selectedItem)>& onConfirm) {
		(new SelectWaypointDialog(titleText, airportsOnly, std::nullopt, onConfirm))->show();
	}

	void SelectWaypointDialog::edit(
		std::wstring_view titleText,
		const WaypointDialogSelectedItem& selectedItem,
		const std::function<void(const WaypointDialogSelectedItem& selectedItem)>& onConfirm
	) {
		const auto& nd = RC::getInstance().getNavigationData();
		const auto& selectedWaypointData = nd.waypoints[selectedItem.waypointIndex];

		(new SelectWaypointDialog(
			titleText,
			selectedWaypointData.type == NavigationWaypointType::airport,
			selectedItem,
			onConfirm
		))->show();
	}

	SelectWaypointDialog::SelectWaypointDialog(
		std::wstring_view titleText,
		bool airportsOnly,
		const std::optional<WaypointDialogSelectedItem>& selectedItem,
		const std::function<void(const WaypointDialogSelectedItem& selectedItem)>& onConfirm
	) {
		auto& rc = RC::getInstance();
		auto& nd = rc.getNavigationData();

		title.setText(titleText);

		// Name
		Theme::apply(&_searchTextField);

		_searchTextField.setPlaceholder(L"Search");

		_searchTextField.textChanged += [this, &nd, airportsOnly] {
			if (!_searchTextField.isFocused())
				return;

			// Searching for waypoints by filter & name
			const auto it =
				_searchTextField.getText().length() == 0
				? nd.waypoints.end()
				: std::ranges::find_if(
					nd.waypoints,
					[this, airportsOnly](const NavigationWaypointData& waypointData) {
						return
							((waypointData.type == NavigationWaypointType::airport) == airportsOnly)
							&& StringUtils::containsIgnoreCase(waypointData.name, _searchTextField.getText());
					}
				);

			// Not found
			if (it == nd.waypoints.end()) {
				_waypointTitle.setVisible(false);
				_runwaysTitle.setVisible(false);
				_runwaysSelector.removeAndDeleteItems();
			}
			// Found
			else {
				_waypointTitle.setVisible(true);

				const auto waypointIndex = std::distance(nd.waypoints.begin(), it);
				const auto& waypointData = nd.waypoints[waypointIndex];

				select(waypointIndex, waypointData, 0);
			}
		};

		rows += &_searchTextFieldTitle;

		// Waypoint
		_waypointButton.setToggle(true);
		_waypointButton.setEnabled(false);
		rows += &_waypointTitle;

		// Runways
		_runwaysSelector.applyDialogStyle();

		rows += &_runwaysTitle;

		// Confirm button
		Theme::applyPrimary(&_confirmButton);
		_confirmButton.setText(L"Confirm");

		_confirmButton.click += [&rc, &nd, this, onConfirm] {
			rc.getApplication().scheduleOnTick([&rc, &nd, this, onConfirm] {
				if (_waypointButton.getWaypointIndex() < 0)
					return;

				const auto& waypointData = nd.waypoints[_waypointButton.getWaypointIndex()];

				// Airport
				if (waypointData.type == NavigationWaypointType::airport) {
					if (_runwaysSelector.getSelectedIndex() < 0)
						return;

					const auto runwayItem = dynamic_cast<RunwayItem*>(_runwaysSelector.getSelectedItem());

					onConfirm(WaypointDialogSelectedItem(_waypointButton.getWaypointIndex(), runwayItem->airportAndRunway));
				}
				else {
					onConfirm(WaypointDialogSelectedItem(_waypointButton.getWaypointIndex(), std::nullopt));
				}

				hide();
				delete this;
			});
		};

		rows += &_confirmButton;

		// Initialization
		if (selectedItem.has_value()) {
			_waypointTitle.setVisible(true);

			const auto& waypointData = nd.waypoints[selectedItem.value().waypointIndex];

			_searchTextField.setText(waypointData.name);

			select(
				selectedItem.value().waypointIndex,
				waypointData,
				selectedItem.value().airportAndRunway.has_value() ? selectedItem.value().airportAndRunway.value().runwayIndex : 0
			);
		}
		else {
			_waypointTitle.setVisible(false);
			_runwaysTitle.setVisible(false);
		}
	}

	void SelectWaypointDialog::select(
		uint16_t waypointIndex,
		const NavigationWaypointData& waypointData,
		uint16_t runwayIndex
	) {
		const auto& nd = RC::getInstance().getNavigationData();

		_waypointButton.setWaypointIndex(waypointIndex);

		// Airport
		if (waypointData.type == NavigationWaypointType::airport) {
			_waypointTitle.title.setText(L"Airport");
			_runwaysTitle.setVisible(true);

			const auto airportIndex = nd.getAirportIndex(waypointIndex);
			const auto& airport = nd.airports[airportIndex];

			_runwaysSelector.removeAndDeleteItems();

			for (uint8_t i = 0; i < airport.runways.size(); i++) {
				_runwaysSelector.addItem(new RunwayItem(NavigationAirportAndRunwayIndicesData(airportIndex, i)));
			}

			_runwaysSelector.setSelectedIndex(runwayIndex);
		}
		// Waypoint
		else {
			_waypointTitle.title.setText(L"Waypoint");
			_runwaysTitle.setVisible(false);
			_runwaysSelector.removeAndDeleteItems();
		}
	}
}
