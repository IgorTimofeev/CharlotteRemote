#include "waypointsPage.h"

#include "rc.h"
#include "utils/navigationData.h"
#include "UI/theme.h"
#include <utils/rendering.h>
#include <utils/string.h>

#include <format>
#include <esp_log.h>
#include <UI/elements/navigation/addWaypointDialog.h>

#include "waypointItem.h"

namespace pizda {
	WaypointsPage* WaypointsPage::_instance = nullptr;

	WaypointsPage::WaypointsPage() {
		_instance = this;

		title.setText(L"Nav database");

		// Type selector
		_airportTypeItem.setText(L"Airport");
		_typeSelector.addItem(&_airportTypeItem);

		_waypointTypeItem.setText(L"Waypoint");
		_typeSelector.addItem(&_waypointTypeItem);

		_typeSelector.setSelectedIndex(0);

		_typeSelector.selectionChanged += [this] {
			updateFromNavigationData();
		};

		rows += &_typeSelector;

		// Search and add button
		_searchAndAddRow.setOrientation(Orientation::horizontal);
		_searchAndAddRow.setSpacing(Theme::spacing);

		// Search text field
		Theme::apply(&_searchTextField);

		_searchTextField.setPlaceholder(L"Search");

		_searchTextField.textChanged += [this] {
			if (_searchTextField.isFocused())
				updateFromNavigationData();
		};

		_searchAndAddRow += &_searchTextField;

		// Add button
		Theme::applySecondary(&_addButton);
		_addButton.setWidth(Theme::elementHeight);
		_addButton.setText(L"+");

		_addButton.click += [this] {
			AddWaypointDialog::create({}, [this] {
				updateFromNavigationData();
			});
		};

		_searchAndAddRow.setAutoSize(&_addButton);
		_searchAndAddRow += &_addButton;

		rows += &_searchAndAddRow;

		// Items
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
		const auto text = _searchTextField.getText();

		_itemsLayout.removeAndDeleteChildren();

		for (uint16_t i = 0; i < nd.waypoints.size(); i++) {
			const auto& waypointData = nd.waypoints[i];

			if (
				(_typeSelector.getSelectedIndex() == 0) == (waypointData.type == NavigationWaypointType::airport)
				&& (
					text.length() == 0
					|| StringUtils::containsIgnoreCase(waypointData.name, text)
				)
			) {
				_itemsLayout += new WaypointItem(i);
			}
		}
	}
}
