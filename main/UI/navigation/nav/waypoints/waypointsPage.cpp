#include "waypointsPage.h"

#include <rc.h>
#include <types/navigationData.h>
#include <UI/theme.h>
#include <utils/rendering.h>
#include <utils/string.h>

#include <format>
#include <esp_log.h>

#include "waypointItem.h"

namespace pizda {
	const WaypointsPage* WaypointsPage::_instance = nullptr;

	WaypointsPage::WaypointsPage() {
		_instance = this;

		title.setText(L"Waypoints");

		Theme::apply(&_searchTextField);

		_searchTextField.textChanged += [this] {
			search();
		};

		rows += &_searchTextField;

		_waypointsRows.setSpacing(5);
		rows += &_waypointsRows;

		updateFromNavigationData();
	}

	WaypointsPage::~WaypointsPage() {
		deleteItems();

		_instance = nullptr;
	}

	const WaypointsPage* WaypointsPage::getInstance() {
		return _instance;
	}

	void WaypointsPage::deleteItems() {
		for (const auto child : _waypointsRows) {
			delete child;
		}
	}

	void WaypointsPage::updateFromNavigationData() {
		const auto& nd = RC::getInstance().getNavigationData();

		// Removing
		deleteItems();

		_waypointsRows.removeChildren();

		// Adding
		for (uint16_t i = 0; i < nd.waypoints.size(); i++) {
			_waypointsRows += new WaypointItem(i);
		}
	}

	void WaypointsPage::search() {
		const auto& nd = RC::getInstance().getNavigationData();
		const auto text = _searchTextField.getText();

		for (const auto child : _waypointsRows) {
			const auto waypointItem = dynamic_cast<WaypointItem*>(child);

			waypointItem->setVisible(
				text.length() == 0
				|| StringUtils::containsIgnoreCase(nd.waypoints[waypointItem->_waypointIndex].name, text)
			);
		}
	}
}
