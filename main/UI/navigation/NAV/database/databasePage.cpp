#include "databasePage.h"

#include "rc.h"
#include "types/navigationData.h"
#include "UI/theme.h"
#include "utilities/rendering.h"
#include <utilities/string.h>

#include <format>
#include <esp_log.h>
#include "UI/elements/navigation/addWaypointDialog.h"

#include "waypointItem.h"

namespace pizda {
	DatabasePage* DatabasePage::_instance = nullptr;

	DatabasePage::DatabasePage() {
		_instance = this;

		title.setText(L"Nav database");

		// Type selector
		_enrouteTypeItem.setText(L"Enroute");
		_typeSelector.addItem(&_enrouteTypeItem);

		_runwayTypeItem.setText(L"Runway");
		_typeSelector.addItem(&_runwayTypeItem);

		_typeSelector.setSelectedIndex(0);

		_typeSelector.setOnSelectionChanged([this] {
			updateFromNavigationData();
		});

		rows += &_typeSelector;

		// Search and add button
		_searchAndAddRow.setOrientation(Orientation::horizontal);
		_searchAndAddRow.setSpacing(Theme::spacing);

		// Search text field
		Theme::apply(&_searchTextField);

		_searchTextField.setPlaceholder(L"Search");

		_searchTextField.setOnTextChanged([this] {
			if (_searchTextField.isFocused())
				updateFromNavigationData();
		});

		_searchAndAddRow += &_searchTextField;

		// Add button
		Theme::applySecondary(&_addButton);
		_addButton.setWidth(Theme::elementHeight);
		_addButton.setText(L"+");

		_addButton.setOnClick([this] {
			AddWaypointDialog::create({}, [this] {
				updateFromNavigationData();
			});
		});

		_searchAndAddRow.setAutoSize(&_addButton);
		_searchAndAddRow += &_addButton;

		rows += &_searchAndAddRow;

		// Items
		_itemsLayout.setSpacing(5);
		rows += &_itemsLayout;

		updateFromNavigationData();
	}

	DatabasePage::~DatabasePage() {
		_itemsLayout.removeAndDeleteChildren();

		_instance = nullptr;
	}

	DatabasePage* DatabasePage::getInstance() {
		return _instance;
	}

	void DatabasePage::updateFromNavigationData() {
		const auto& nd = RC::getInstance().getNavigationData();
		const auto text = _searchTextField.getText();

		_itemsLayout.removeAndDeleteChildren();

		for (uint16_t i = 0; i < nd.waypoints.size(); i++) {
			const auto& waypointData = nd.waypoints[i];

			if (
				(_typeSelector.getSelectedIndex() == 1) == (waypointData.type == NavigationWaypointType::runway)
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
