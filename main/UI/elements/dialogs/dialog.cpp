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

		if (event->getTypeID() == PointerDownEvent::typeID) {
			setFocused(true);
		}
		else if (event->getTypeID() == PointerUpEvent::typeID && isFocused()) {
			RC::getInstance().getApplication().invokeOnNextTick([this] {
				const auto dialog = dynamic_cast<Dialog*>(getParent());
				
				dialog->hide();
				delete dialog;
			});
		}

		event->setHandled(true);
	}

	Dialog::Dialog() {
		// Dithered
		*this += ditheredBackground;

		// Background
		slideLayoutBackground.setFillColor(&Theme::bg2);
		slideLayout += slideLayoutBackground;

		// Slide
		slideLayout.setMaxHeight(Application::getCurrent().getRenderer().getTarget().getSize().getHeight() * 9 / 10);
		setAutoSize(slideLayout);
		*this += slideLayout;
	}

	void Dialog::show() {
		Application::getCurrent() += *this;
	}

	void Dialog::hide() {
		*getParent() -= *this;
	}
}
