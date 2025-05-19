#pragma once

#include <types/navigationData.h>
#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/elements/dialogs/dialog.h>

namespace pizda {
	using namespace YOBA;

	class AirportFlightPlanItemDialog : public Dialog {
		public:
			explicit AirportFlightPlanItemDialog(const NavigationDataFlightPlanAirport& flightPlanAirport, bool destination);

		private:
			Button _removeButton {};
	};
}
