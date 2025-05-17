#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/theme.h>

namespace pizda {
	using namespace YOBA;

	class DialogSlideLayoutBackground : public DitheredRectangle {
		public:
			DialogSlideLayoutBackground() {
				setFillColor(&Theme::bg1);
			}

		protected:
			void onEvent(Event* event) override {
				Element::onEvent(event);

				if (!ScreenEvent::isScreen(event))
					return;

				event->setHandled(true);

				reinterpret_cast<ModalElement*>(getParent())->hide();
			}
	};

	class Dialog : public RelativeStackLayout, public ModalElement {
		public:
			Dialog() {
				// Dithered
				*this += &ditheredBackground;

				// Slide
				slideLayout.setVerticalAlignment(Alignment::end);
				setAutoSize(&slideLayout);
				*this += &slideLayout;

				// Background
				slideLayoutBackground.setFillColor(&Theme::bg2);
				slideLayout += &slideLayoutBackground;

				// Title
				Theme::apply(&title);
				rows += &title;

				// Rows
				rows.setMargin(Margin(15));
				rows.setSpacing(10);
				slideLayout += &rows;
			}

			DialogSlideLayoutBackground ditheredBackground {};

			Rectangle slideLayoutBackground {};
			Layout slideLayout {};

			StackLayout rows {};
			Text title {};
	};
}