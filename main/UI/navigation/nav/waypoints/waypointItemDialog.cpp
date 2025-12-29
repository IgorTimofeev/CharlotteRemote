#include "waypointItemDialog.h"

#include "rc.h"
#include "utils/navigationData.h"
#include "UI/theme.h"

#include "waypointsPage.h"

namespace pizda {
	WaypointItemDialog::WaypointItemDialog(uint16_t waypointIndex) {
		auto& rc = RC::getInstance();
		auto& nd = rc.getNavigationData();

		const auto& waypointData = nd.waypoints[waypointIndex];

		title.setText(waypointData.name);

		// Remove button
		Theme::applyCritical(&_removeButton);
		_removeButton.setText(L"Delete");

		_removeButton.click += [&rc, &nd, waypointIndex, this] {
			rc.getApplication().scheduleOnTick([&rc, &nd, waypointIndex, this] {
				hide();
				delete this;

				nd.removeWaypointAt(waypointIndex);

				const auto page = WaypointsPage::getInstance();

				if (page)
					page->updateFromNavigationData();
			});
		};

		rows += &_removeButton;
	}
}