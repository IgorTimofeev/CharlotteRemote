#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/elements/dialogs/scrollViewDialog.h>

namespace pizda {
	using namespace YOBA;

	class LegFlightPlanItemDialog : public ScrollViewDialog {
		public:
			explicit LegFlightPlanItemDialog(uint16_t legIndex);

			static void showWaypointSelectionDialogToInsertAt(
				std::wstring_view title,
				size_t insertAt
			);

		private:
			Button _editButton {};
			Button _insertBeforeButton {};
			Button _insertAfterButton {};
			Button _removeButton {};

			void insertOnButtonClick(const std::wstring& title, uint16_t insertAt);
	};
}
