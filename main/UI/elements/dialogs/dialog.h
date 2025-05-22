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

				if (event->getTypeID() != TouchDownEvent::typeID)
					return;

				event->setHandled(true);

				const auto element = dynamic_cast<ModalElement*>(getParent());

				element->hide();
				delete element;
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

				// Slide
				slideLayout.setMaxHeight(Application::getCurrent()->getRenderer()->getTarget()->getSize().getHeight() * 8 / 10);
				setAutoSize(&slideLayout);
				*this += &slideLayout;
			}

			DialogSlideLayoutBackground ditheredBackground {};

			Rectangle slideLayoutBackground {};
			Layout slideLayout {};
	};

	class ScrollViewDialog : public Dialog {
		public:
			ScrollViewDialog() {
				// Rows
				rows.setMargin(Margin(15));
				rows.setSpacing(Theme::spacing);

				// Title
				Theme::applyPageTitle(&title);
				rows += &title;

				// ScrollView
				Theme::apply(&scrollView);
				scrollView += &rows;
				slideLayout += &scrollView;
			}

			ScrollView scrollView {};
			StackLayout rows {};
			Text title {};
	};
}