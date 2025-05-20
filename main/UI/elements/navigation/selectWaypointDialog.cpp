#include "selectWaypointDialog.h"

#include <rc.h>
#include <UI/theme.h>
#include <utils/string.h>
#include <esp_log.h>
#include <esp_log.h>

namespace pizda {
	RunwayItem::RunwayItem(const NavigationAirportIndexAndRunwayIndexData& airportAndRunway) : airportAndRunway(airportAndRunway) {
		setHeight(WaypointButton::height);
	}

	void RunwayItem::onRender(Renderer* renderer) {
		const auto& bounds = getBounds();

		const auto& airport = RC::getInstance().getNavigationData().airports[airportAndRunway.airportIndex];
		const auto& runway = airport.runways[airportAndRunway.runwayIndex];

		renderer->renderFilledRectangle(
			bounds,
			Theme::cornerRadius,
			isActive() ? &Theme::bg4 : &Theme::bg3
		);

		renderer->renderRectangle(
			bounds,
			Theme::cornerRadius,
			isActive() ? &Theme::fg1 : &Theme::bg4
		);

		// Name
		const auto text =runway.getFormattedName();

		renderer->renderString(
			Point(
				bounds.getXCenter() - Theme::fontNormal.getWidth(text) / 2,
				bounds.getYCenter() - Theme::fontNormal.getHeight() / 2
			),
			&Theme::fontNormal,
			&Theme::fg1,
			text
		);
	}

	SelectWaypointDialog::SelectWaypointDialog(
		std::wstring_view titleText,
		const std::function<bool(const NavigationWaypointData&)>& filter,
		const std::function<void(uint16_t, const std::optional<NavigationAirportIndexAndRunwayIndexData>& airportAndRunway)>& onConfirm
	) {
		auto& rc = RC::getInstance();
		auto& nd = rc.getNavigationData();

		title.setText(titleText);

		// Name
		Theme::apply(&_name);

		_name.textChanged += [this, &nd, filter] {
			// Searching for waypoints by filter & name
			const auto it =
				_name.getText().length() == 0
				? nd.waypoints.end()
				: std::ranges::find_if(
					nd.waypoints,
					[this, filter](const NavigationWaypointData& waypointData) {
						return
							filter(waypointData)
							&& StringUtils::containsIgnoreCase(waypointData.name, _name.getText());
					}
				);

			// Not found
			if (it == nd.waypoints.end()) {
				_waypointTitle.getTitle().setText(L"Waypoint");

				_waypointPlaceholder.setVisible(true);

				_waypointButton.setVisible(false);

				_runwaysTitle.setVisible(false);

				_runwaysSelector.setSelectedIndex(-1);
				_runwaysLayout.removeAndDeleteChildren();
			}
			// Found
			else {
				const auto waypointIndex = std::distance(nd.waypoints.begin(), it);
				const auto& waypointData = nd.waypoints[waypointIndex];

				_waypointPlaceholder.setVisible(false);

				_waypointButton.setWaypointIndex(waypointIndex);
				_waypointButton.setVisible(true);

				// Airport
				if (waypointData.type == NavigationWaypointType::airport) {
					_waypointTitle.getTitle().setText(L"Airport");

					_runwaysTitle.setVisible(true);

					_runwaysSelector.setSelectedIndex(-1);
					_runwaysLayout.removeAndDeleteChildren();

					const auto airportIndex = nd.getAirportIndex(waypointIndex);
					const auto& airport = nd.airports[airportIndex];

					for (uint8_t runwayIndex = 0; runwayIndex < airport.runways.size(); runwayIndex++) {
						_runwaysSelector.addItem(new RunwayItem(NavigationAirportIndexAndRunwayIndexData(airportIndex, runwayIndex)));
					}

					_runwaysSelector.setSelectedIndex(0);
				}
				// Waypoint
				else {
					_waypointTitle.getTitle().setText(L"Waypoint");

					_runwaysTitle.setVisible(false);

					_runwaysSelector.setSelectedIndex(-1);
					_runwaysLayout.removeAndDeleteChildren();
				}
			}
		};

		rows += &_nameTitle;

		// Waypoint
		Theme::applyPlaceholder(&_waypointPlaceholder);
		_waypointPlaceholder.setHeight(WaypointButton::height);
		_waypointPlaceholder.setText(L"Press me, senpai");
		_waypointLayout += &_waypointPlaceholder;

		_waypointButton.setVisible(false);
		_waypointButton.setToggle(true);
		_waypointButton.setEnabled(false);
		_waypointLayout += &_waypointButton;

		rows += &_waypointTitle;

		// Runways
		_runwaysTitle.setVisible(false);

		_runwaysLayout.setOrientation(Orientation::horizontal);
		_runwaysLayout.setSpacing(5);
		_runwaysSelector += &_runwaysLayout;
		_runwaysSelector.setItemsLayout(&_runwaysLayout);

		rows += &_runwaysTitle;

		// Confirm button
		Theme::apply(&_confirmButton);
		_confirmButton.setText(L"Confirm");

		_confirmButton.click += [&rc, &nd, this, onConfirm, filter] {
			rc.getApplication().scheduleOnTick([&rc, &nd, this, onConfirm, filter] {
				if (_waypointButton.getWaypointIndex() < 0)
					return;

				const auto& waypointData = nd.waypoints[_waypointButton.getWaypointIndex()];

				// Airport
				if (waypointData.type == NavigationWaypointType::airport) {
					if (_runwaysSelector.getSelectedIndex() < 0)
						return;

					const auto runwayItem = dynamic_cast<RunwayItem*>(_runwaysSelector.getSelectedItem());

					onConfirm(_waypointButton.getWaypointIndex(), runwayItem->airportAndRunway);
				}
				else {
					onConfirm(_waypointButton.getWaypointIndex(), std::nullopt);
				}

				hide();
				delete this;
			});
		};

		rows += &_confirmButton;
	}
}
