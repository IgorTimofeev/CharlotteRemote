#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "dialog.h"

namespace pizda {
	using namespace YOBA;

	class CancelConfirmDialog : public Dialog {
		public:
			CancelConfirmDialog() {
				// Cancel
				Theme::applySecondary(&cancelButton);
				cancelButton.setText(L"Cancel");
				bottomButtonsRow += &cancelButton;

				// Confirm
				Theme::applyPrimary(&confirmButton);
				confirmButton.setText(L"Confirm");
				bottomButtonsRow += &confirmButton;
			}

			RelativeStackLayout bottomButtonsRow { Orientation::horizontal, 10 };
			Button cancelButton {};
			Button confirmButton {};
	};
}