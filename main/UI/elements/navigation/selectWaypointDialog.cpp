#include "selectWaypointDialog.h"

#include <rc.h>
#include <UI/theme.h>
#include <utils/string.h>
#include <esp_log.h>

namespace pizda {
	SelectWaypointDialog::SelectWaypointDialog(
		std::wstring_view titleText,
		const std::function<bool(const NavigationWaypointData&)>& filter,
		const std::function<void(uint16_t)>& onConfirm
	) {
		auto& rc = RC::getInstance();
		auto& nd = rc.getNavigationData();

		title.setText(titleText);

		// Name
		Theme::apply(&_name);
		rows += &_nameTitle;

		// Confirm button
		Theme::apply(&_confirmButton);
		_confirmButton.setText(L"Confirm");

		_confirmButton.click += [&rc, &nd, this, onConfirm, filter] {
			rc.getApplication().scheduleTask([&rc, &nd, this, onConfirm, filter] {
				if (_name.getText().length() == 0)
					return;

				const auto it = std::ranges::find_if(nd.waypoints, [this, filter](const NavigationWaypointData& waypointData) {
					return filter(waypointData) && StringUtils::containsIgnoreCase(waypointData.name, _name.getText());
				});

				if (it != nd.waypoints.end()) {
					const uint16_t waypointIndex = std::distance(nd.waypoints.begin(), it);

					onConfirm(waypointIndex);

					hide();
					delete this;
				}
			});
		};

		rows += &_confirmButton;
	}
}
