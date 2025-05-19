#include "selectWaypointDialog.h"

#include <rc.h>
#include <UI/theme.h>
#include <utils/string.h>
#include <esp_log.h>
#include <esp_log.h>

namespace pizda {
	RunwayButton::RunwayButton(const NavigationAirportIndexAndRunwayIndexData& airportAndRunway) : airportAndRunway(airportAndRunway) {
		setHeight(WaypointButton::height);
		setToggle(true);
	}

	void RunwayButton::onRender(Renderer* renderer) {
		const auto& bounds = getBounds();

		const auto& airport = RC::getInstance().getNavigationData().airports[airportAndRunway.airportIndex];
		const auto& runway = airport.runways[airportAndRunway.runwayIndex];

		constexpr static uint8_t cornerRadius = 3;

		renderer->renderFilledRectangle(
			bounds,
			Theme::cornerRadius,
			isActive() ? &Theme::bg4 : &Theme::bg3
		);

		renderer->renderRectangle(
			bounds,
			cornerRadius,
			isActive() ? &Theme::fg1 : &Theme::bg4
		);

		// Name
		renderer->renderString(
			Point(
				bounds.getX() + 15,
				bounds.getYCenter() - Theme::fontNormal.getHeight() / 2
			),
			&Theme::fontNormal,
			&Theme::fg1,
			runway.getFormattedName()
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
				_runwaysLayout.removeAndDeleteChildren();
			}
			// Found
			else {
				const auto waypointIndex = std::distance(nd.waypoints.begin(), it);
				const auto& waypointData = nd.waypoints[waypointIndex];

				_waypointPlaceholder.setVisible(false);

				_waypointButton.setWaypointIndex(waypointIndex);
				_waypointButton.setActive(false);
				_waypointButton.setVisible(true);

				// Airport
				if (waypointData.type == NavigationWaypointType::airport) {
					_waypointTitle.getTitle().setText(L"Airport");

					_runwaysTitle.setVisible(true);
					_runwaysLayout.removeAndDeleteChildren();

					const auto airportIndex = nd.getAirportIndex(waypointIndex);
					const auto& airport = nd.airports[airportIndex];

					for (size_t runwayIndex = 0; runwayIndex < airport.runways.size(); runwayIndex++) {
						_runwaysLayout += new RunwayButton(NavigationAirportIndexAndRunwayIndexData(airportIndex, runwayIndex));
					}
				}
				// Waypoint
				else {
					_waypointTitle.getTitle().setText(L"Waypoint");

					_runwaysTitle.setVisible(false);
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
		_waypointLayout += &_waypointButton;

		rows += &_waypointTitle;

		// Runways
		_runwaysTitle.setVisible(false);
		_runwaysLayout.setSpacing(5);
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
					if (_runwaysLayout.getChildrenCount() == 0)
						return;

					const auto runwayButton = dynamic_cast<RunwayButton*>(_runwaysLayout[0]);

					onConfirm(_waypointButton.getWaypointIndex(), runwayButton->airportAndRunway);
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
