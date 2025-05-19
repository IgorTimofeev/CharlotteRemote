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

				// Background
				slideLayoutBackground.setFillColor(&Theme::bg2);
				slideLayout += &slideLayoutBackground;

				// Rows
				rows.setMargin(Margin(15));
				rows.setSpacing(10);

				// Title
				Theme::apply(&title);
				rows += &title;

				// ScrollView
				Theme::apply(&scrollView);
				scrollView += &rows;
				slideLayout += &scrollView;

				// Slide
				slideLayout.setMaxHeight(Application::getCurrent()->getRenderer()->getTarget()->getSize().getHeight() * 8 / 10);
				setAutoSize(&slideLayout);
				*this += &slideLayout;
			}

			DialogSlideLayoutBackground ditheredBackground {};

			Rectangle slideLayoutBackground {};
			Layout slideLayout {};

			ScrollView scrollView {};
			StackLayout rows {};
			Text title {};
	};
}