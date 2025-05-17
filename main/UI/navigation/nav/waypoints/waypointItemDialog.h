#pragma once

#include <esp_log.h>
#include <UI/elements/dialogs/dialog.h>
#include <UI/theme.h>
#include <UI/elements/titler.h>
#include <types/navigationData.h>

namespace pizda {
	using namespace YOBA;

	class WaypointItemDialog : public Dialog {
		public:
			Button directToButton {};
			Button addToFlightPlanButton {};
			Button removeButton {};

			static void create(const NavigationWaypointData* waypointData) {
				const auto dialog = new WaypointItemDialog(waypointData);
				dialog->show();
			}

		private:
			explicit WaypointItemDialog(const NavigationWaypointData* waypointData) {
				title.setText(waypointData->name);

				Theme::applySecondary(&directToButton);
				directToButton.setText(L"Direct to");
				rows += &directToButton;

				Theme::applySecondary(&addToFlightPlanButton);
				addToFlightPlanButton.setText(L"Add to FPL");
				rows += &addToFlightPlanButton;

				Theme::applyCritical(&removeButton);
				removeButton.setText(L"Delete");
				rows += &removeButton;
			}
	};
}