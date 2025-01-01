#include "debugPage.h"
#include "../../../../lib/YOBA/src/ui/keyboard.h"
#include "../../../../lib/YOBA/src/ui/modal.h"
#include "../../../../lib/YOBA/src/ui/application.h"

namespace pizdanc {
	DebugPage::DebugPage() {
		rows.setWidth(140);
		rows.setAlignment(Alignment::Center);

		// Slider
		Theme::apply(slider);
		slider.setValue(0);
		rows += &sliderTitle;

		// Button 1
		Theme::apply(button1);
		button1.setText(L"Click");
		rows += &button1Title;

		button1.getOnClick() += [this]() {
			auto overlay = new ModalOverlay();
			auto root = button1.getRoot();

			auto keyboard = new Keyboard({
				[]() {
					return new EnglishKeyboardLayout();
				}
			});

			keyboard->setVerticalAlignment(Alignment::End);
			keyboard->setBackgroundColor(&Theme::bg3);
			keyboard->setTextButtonPrimaryColor(&Theme::bg5);
			keyboard->setTextButtonSecondaryColor(&Theme::fg1);
			keyboard->setActionButtonPrimaryColor(&Theme::bg4);
			keyboard->setActionButtonSecondaryColor(&Theme::fg1);
			keyboard->setLayoutIndex(0);

			overlay->getBackgroundRectangle().getEventHandlers() += [root, keyboard, overlay](InputEvent& event) {
				if (event.getTypeID() != TouchDownEvent::typeID)
					return;

				event.setHandled(true);

				Serial.printf("Pizda: %p, %p, %p\n", root, keyboard, overlay);

				*root -= overlay;
				delete keyboard;
				delete overlay;
			};

			*overlay += keyboard;

			Serial.println("ADDED BLYAD");
			*root += overlay;
		};
	}
}