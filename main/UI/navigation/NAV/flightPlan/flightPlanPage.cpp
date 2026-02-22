#include "flightPlanPage.h"

#include "rc.h"
#include "flightPlanItem.h"

namespace pizda {
	FlightPlanPage* FlightPlanPage::_instance = nullptr;

	FlightPlanPage::FlightPlanPage() {
		_instance = this;

		title.setText(L"Flight plan");

		_itemsLayout.setGap(5);
		rows += _itemsLayout;

		// Legs
		_legsLayout.setGap(5);
		_itemsLayout += _legsLayout;

		// Add leg
		_addLegButton.setText(L"Add leg");
		_itemsLayout += _addLegButton;

		updateFromNavigationData();
	}

	FlightPlanPage::~FlightPlanPage() {
		_instance = nullptr;

		_legsLayout.removeAndDeleteChildren();
	}

	FlightPlanPage* FlightPlanPage::getInstance() {
		return _instance;
	}

	void FlightPlanPage::applyTitleStyle(TextView& text) {
		Theme::apply(&text);
		text.setTextColor(Theme::fg4);
	}

	void FlightPlanPage::updateFromNavigationData() {
		const auto& nd = RC::getInstance().getNavigationData();

		// Legs
		_legsLayout.removeAndDeleteChildren();
	
		for (uint16_t i = 0; i < nd.flightPlan.legs.size(); i++)
			_legsLayout += *new FlightPlanItem(i);
	}
}
