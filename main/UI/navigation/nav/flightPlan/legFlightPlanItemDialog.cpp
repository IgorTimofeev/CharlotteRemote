#include "legFlightPlanItemDialog.h"

#include <rc.h>
#include <types/navigationData.h>
#include <UI/theme.h>

#include "flightPlanPage.h"

namespace pizda {
	LegFlightPlanItemDialog::LegFlightPlanItemDialog(uint16_t legIndex) {
		auto& rc = RC::getInstance();
		auto& nd = rc.getNavigationData();

		const auto& leg = nd.flightPlan.legs[legIndex];
		const auto& waypointData = nd.waypoints[leg.waypointIndex];

		title.setText(waypointData.name);

		// Insert before
		Theme::applySecondary(&_insertBeforeButton);
		_insertBeforeButton.setText(L"Insert before");

		_insertBeforeButton.click += [&rc, &nd, this, &leg] {
			rc.getApplication().scheduleTask([&rc, &nd, this, &leg] {


				hide();
				delete this;
			});
		};

		rows += &_insertBeforeButton;

		// Insert after
		Theme::applySecondary(&_insertAfterButton);
		_insertAfterButton.setText(L"Insert after");

		_insertAfterButton.click += [&rc, &nd, this, &leg] {
			rc.getApplication().scheduleTask([&rc, &nd, this, &leg] {


				hide();
				delete this;
			});
		};

		rows += &_insertAfterButton;

		// Remove
		Theme::applyCritical(&_removeButton);
		_removeButton.setText(L"Delete");

		_removeButton.click += [&rc, &nd, this, &leg] {
			rc.getApplication().scheduleTask([&rc, &nd, this, &leg] {
				nd.flightPlan.legs.erase(nd.flightPlan.legs.begin() + leg.waypointIndex);

				const auto page = FlightPlanPage::getInstance();

				if (page)
					page->updateFromNavigationData();

				hide();
				delete this;
			});
		};

		rows += &_removeButton;
	}
}