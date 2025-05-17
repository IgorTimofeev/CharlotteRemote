#include "waypointsPage.h"

#include <rc.h>
#include <types/navigationData.h>
#include <UI/theme.h>
#include <utils/rendering.h>

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
		_instance = nullptr;
	}

	const WaypointsPage* WaypointsPage::getInstance() {
		return _instance;
	}

	void WaypointsPage::updateFromNavigationData() {
		const auto& nd = RC::getInstance().getNavigationData();

		// Removing
		for (const auto child : _waypointsRows) {
			delete child;
		}

		_waypointsRows.removeChildren();

		// Adding
		for (const auto& waypoint : nd.waypoints) {
			_waypointsRows += new WaypointItem(&waypoint);
		}
	}

	void WaypointsPage::search() {
		auto text = std::wstring(_searchTextField.getText());

		std::ranges::transform(text, text.begin(), ::towlower);

		for (const auto child : _waypointsRows) {
			const auto waypointItem = dynamic_cast<WaypointItem*>(child);

			std::wstring pizda = waypointItem->waypointData->name;
			std::ranges::transform(pizda, pizda.begin(), ::towlower);

			waypointItem->setVisible(text.length() == 0 || pizda.contains(text));
		}
	}
}
