#include "flightPlanPage.h"

#include "rc.h"

#include <numbers>
#include <format>
#include <esp_log.h>

#include "flightPlanItem.h"

namespace pizda {
	FlightPlanPage* FlightPlanPage::_instance = nullptr;

	FlightPlanPage::FlightPlanPage() {
		_instance = this;

		title.setText(L"Flight plan");

		_itemsLayout.setSpacing(5);
		rows += &_itemsLayout;

		// Origin
		applyTitleStyle(_originTitle);
		_originTitle.setText(L"Origin");
		_itemsLayout += &_originTitle;

		_itemsLayout += &_originPlaceholderItem;
		_itemsLayout += &_originItem;

		// Legs
		applyTitleStyle(_legsTitle);
		_legsTitle.setText(L"Route");
		_itemsLayout += &_legsTitle;

		_itemsLayout += &_legPlaceholderButton;

		_legsLayout.setSpacing(5);
		_itemsLayout += &_legsLayout;

		// Destination
		applyTitleStyle(_destinationTitle);
		_destinationTitle.setText(L"Destination");
		_itemsLayout += &_destinationTitle;

		_itemsLayout += &_destinationPlaceholderItem;
		_itemsLayout += &_destinationItem;

		updateFromNavigationData();
	}

	FlightPlanPage::~FlightPlanPage() {
		_instance = nullptr;

		_legsLayout.removeAndDeleteChildren();
	}

	FlightPlanPage* FlightPlanPage::getInstance() {
		return _instance;
	}

	void FlightPlanPage::applyTitleStyle(Text& text) {
		Theme::apply(&text);
		text.setTextColor(&Theme::fg4);
	}

	void FlightPlanPage::updateFromNavigationData() {
		const auto& nd = RC::getInstance().getNavigationData();

		// Origin
		_originItem.setVisible(nd.flightPlan.origin.has_value());
		_originItem.setAirportAndRunway(nd.flightPlan.origin);
		_originPlaceholderItem.setVisible(!nd.flightPlan.origin.has_value());

		// Legs
		_legPlaceholderButton.setVisible(nd.flightPlan.legs.size() == 0);

		_legsLayout.removeAndDeleteChildren();
	
		for (uint16_t i = 0; i < nd.flightPlan.legs.size(); i++)
			_legsLayout += new LegFlightPlanItem(i);

		// Destination
		_destinationItem.setVisible(nd.flightPlan.destination.has_value());
		_destinationItem.setAirportAndRunway(nd.flightPlan.destination);
		_destinationPlaceholderItem.setVisible(!nd.flightPlan.destination.has_value());
	}
}
