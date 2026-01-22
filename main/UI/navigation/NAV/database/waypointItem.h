#pragma once

#include "UI/elements/navigation/waypointButton.h"
#include "waypointItemDialog.h"

namespace pizda {
	class WaypointItem : public WaypointButton {
		public:
			explicit WaypointItem(uint32_t waypointIndex) : WaypointButton(waypointIndex) {

			}

		protected:
			void onClick() override {
				if (getWaypointIndex() < 0)
					return;

				(new WaypointItemDialog(getWaypointIndex()))->show();
			}
	};
}
