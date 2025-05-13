#pragma once

#include <YOBA/UI.h>
#include "../theme.h"

namespace pizda {
	using namespace YOBA;

	class Dialog : public ModalLayout {
		public:
			Dialog() {
				// Background rect
				*this += &backgroundRectangle;

				// Working area rect
				workingAreaRectangle.setCornerRadius(5);
				workingAreaRectangle.setFillColor(&Theme::bg2);
				workingAreaRectangle.setBorderColor(&Theme::bg4);
				workingAreaLayout += &workingAreaRectangle;

				// Title
				Theme::apply(&title);
				rows += &title;

				// Rows
				rows.setMargin(Margin(13));
				rows.setSpacing(10);
				workingAreaLayout += &rows;

				// Working area layout
				workingAreaLayout.setVerticalAlignment(Alignment::center);
				workingAreaLayout.setMargin(Margin(20, 0));
				*this += &workingAreaLayout;
			}

			Rectangle backgroundRectangle;
			Layout workingAreaLayout;
			Rectangle workingAreaRectangle;
			StackLayout rows;
			Text title;
	};

	class ProgressDialog : public Dialog {
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

			void setProgress(uint16_t value) {
				progressBar.setValue(value);
				progressText.setText(std::format(L"{}%", value * 100 / 0xFFFF));
			}
	};
}