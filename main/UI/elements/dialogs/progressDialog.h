#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "UI/elements/dialogs/scrollViewDialog.h"

namespace pizda {
	using namespace YOBA;

	class ProgressDialog : public ScrollViewDialog {
		public:
			ProgressDialog() {
				// Description text
				Theme::applyDescription(&descriptionText);
				descriptionText.setWrappingEnabled(true);
				rows += &descriptionText;

				// Progress bar
				Theme::apply(&progressBar);
				rows += &progressBar;

				// Progress text
				Theme::apply(&progressText);
				progressText.setHorizontalAlignment(Alignment::center);
				rows += &progressText;

				setProgress(0);
			}

			ProgressBar progressBar;
			Text progressText;
			Text descriptionText;

			void setProgress(float value) {
				progressBar.setValue(value);
				progressText.setText(std::format(L"{}%", value * 100 / 0xFFFF));
			}
	};
}