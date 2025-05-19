#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/elements/dialogs/dialog.h>

namespace pizda {
	using namespace YOBA;

	class LegFlightPlanItemDialog : public Dialog {
		public:
			explicit LegFlightPlanItemDialog(uint16_t legIndex);

			static void showWaypointSelectionDialogToInsertAt(
				std::wstring_view title,
				size_t insertAt
			);

		private:
			Button _insertBeforeButton {};
			Button _insertAfterButton {};
			Button _removeButton {};

			void insertWaypoint(const std::wstring& title, uint16_t insertAt);
	};
}
