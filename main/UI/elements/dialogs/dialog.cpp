#include "dialog.h"

#include <rc.h>

namespace pizda {
	DialogSlideLayoutBackground::DialogSlideLayoutBackground() {
		setFillColor(&Theme::bg1);
	}

	void DialogSlideLayoutBackground::onTouchUp(TouchUpEvent* event) {
		event->setHandled(true);

		RC::getInstance().getApplication().scheduleOnTick([this] {
			const auto element = dynamic_cast<ModalElement*>(getParent());

			element->hide();
			delete element;
		});
	}

	Dialog::Dialog() {
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

	ScrollViewDialog::ScrollViewDialog() {
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
}
