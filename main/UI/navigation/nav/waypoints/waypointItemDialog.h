#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/elements/dialogs/dialog.h>

namespace pizda {
	using namespace YOBA;

	class WaypointItemDialog : public ScrollViewDialog {
		public:
			explicit WaypointItemDialog(uint16_t waypointIndex);

		private:
			Button _removeButton {};
	};
}
