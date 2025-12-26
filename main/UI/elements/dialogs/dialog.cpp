#include "dialog.h"

#include "rc.h"
#include "UI/theme.h"

namespace pizda {
	DialogSlideLayoutBackground::DialogSlideLayoutBackground() {
		setFillColor(&Theme::bg1);
	}

	void DialogSlideLayoutBackground::onEvent(Event* event) {
		if (!ScreenEvent::isScreen(event))
			return;

		event->setHandled(true);

		if (event->getTypeID() != PointerUpEvent::typeID)
			return;

		RC::getInstance().getApplication().scheduleOnTick([this] {
			const auto element = dynamic_cast<Dialog*>(getParent());

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

	void Dialog::show() {
		*Application::getCurrent() += this;
	}

	void Dialog::hide() {
		*getParent() -= this;
	}
}
