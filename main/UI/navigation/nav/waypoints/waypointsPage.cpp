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
	WaypointsPage* WaypointsPage::_instance = nullptr;

	WaypointsPage::WaypointsPage() {
		_instance = this;

		title.setText(L"Waypoints");

		Theme::apply(&_searchTextField);

		_searchTextField.textChanged += [this] {
			search();
		};

		rows += &_searchTextField;

		_itemsLayout.setSpacing(5);
		rows += &_itemsLayout;

		updateFromNavigationData();
	}

	WaypointsPage::~WaypointsPage() {
		_itemsLayout.removeAndDeleteChildren();

		_instance = nullptr;
	}

	WaypointsPage* WaypointsPage::getInstance() {
		return _instance;
	}

	void WaypointsPage::updateFromNavigationData() {
		const auto& nd = RC::getInstance().getNavigationData();

		_itemsLayout.removeAndDeleteChildren();

		for (uint16_t i = 0; i < nd.waypoints.size(); i++)
			_itemsLayout += new WaypointItem(i);
	}

	void WaypointsPage::search() {
		const auto& nd = RC::getInstance().getNavigationData();
		const auto text = _searchTextField.getText();

		for (const auto child : _itemsLayout) {
			const auto waypointItem = dynamic_cast<WaypointItem*>(child);

			waypointItem->setVisible(
				text.length() == 0
				|| StringUtils::containsIgnoreCase(nd.waypoints[waypointItem->getWaypointIndex()].name, text)
			);
		}
	}
}
