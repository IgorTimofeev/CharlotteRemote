#pragma once

#include <types/navigationData.h>
#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/elements/dialogs/dialog.h>

namespace pizda {
	using namespace YOBA;

	class AirportFlightPlanItemDialog : public ScrollViewDialog {
		public:
			explicit AirportFlightPlanItemDialog(const NavigationAirportAndRunwayIndicesData& airportAndRunway, bool destination);

		private:
			Button _editButton {};
			Button _removeButton {};
	};
}
