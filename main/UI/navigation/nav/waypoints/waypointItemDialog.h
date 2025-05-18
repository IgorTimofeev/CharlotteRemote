#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/elements/dialogs/dialog.h>

namespace pizda {
	using namespace YOBA;

	class WaypointItemDialog : public Dialog {
		public:
			Button FPLOriginButton {};
			Button FPLLegButton {};
			Button FPLDestinationButton {};
			Button removeButton {};

			static void showCyka(uint16_t waypointIndex);

		private:
			explicit WaypointItemDialog(uint16_t waypointIndex);
	};
}
