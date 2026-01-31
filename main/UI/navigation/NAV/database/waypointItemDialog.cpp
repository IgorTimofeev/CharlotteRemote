#include "waypointItemDialog.h"

#include "rc.h"
#include "types/navigationData.h"
#include "UI/theme.h"

#include "databasePage.h"

namespace pizda {
	WaypointItemDialog::WaypointItemDialog(uint16_t waypointIndex) {
		auto& rc = RC::getInstance();

		const auto& waypointData = rc.getNavigationData().waypoints[waypointIndex];

		title.setText(waypointData.name);

		// Remove button
		Theme::applyCritical(&_removeButton);
		_removeButton.setText(L"Delete");

		_removeButton.setOnClick([&rc, waypointIndex, this] {
			rc.getApplication().scheduleOnTick([&rc, waypointIndex, this] {
				hide();
				delete this;

				rc.getNavigationData().removeWaypointAt(waypointIndex);

				const auto page = DatabasePage::getInstance();

				if (page)
					page->updateFromNavigationData();
			});
		});

		rows += &_removeButton;
	}
}