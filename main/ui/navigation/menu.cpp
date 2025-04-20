#include "menu.h"
#include "../../rc.h"

namespace pizda {
	MenuOverlayBackground::MenuOverlayBackground() {
		setFillColor(&Theme::bg1);
	}

	void MenuOverlayBackground::onEvent(Event* event) {
		Element::onEvent(event);

		if (!ScreenEvent::isScreen(event))
			return;

		event->setHandled(true);

		RC::getInstance().setMenuVisibility(false);
	}

	Menu::Menu() {
		// Background
		*this += &_overlayBackground;

		// Holder
		_slideLayout.setVerticalAlignment(Alignment::end);

		// Items background
		_slideBackground.setFillColor(&Theme::bg2);
		_slideLayout += &_slideBackground;

		// Titles & buttons

		// Main
		addTitle(&_MFDTitle);

		addRouteButton(&_mainWL, &_mainMFDButton);
		addOptionButton(&_mainWL, &_mainNDButton);
		addOptionButton(&_mainWL, &_mainAutopilotButton);
		addOptionButton(&_mainWL, &_mainPressureButton);

		addWrapLayout(&_mainWL);

		// Settings
		addTitle(&_settingsTitle);

		addRouteButton(&_settingsWL, &_settingsAxisButton);
		addRouteButton(&_settingsWL, &_settingsWiFiButton);
		addRouteButton(&_settingsWL, &_developerUITestButton);

		addWrapLayout(&_settingsWL);

		_slideRows.setSpacing(10);
		_slideRows.setMargin(Margin(15, 10, 15, 10));
		_slideLayout += &_slideRows;

		setAutoSize(&_slideLayout);
		*this += &_slideLayout;
	}

	void Menu::addTitle(Text* text) {
		Theme::apply(text);

		_slideRows += text;
	}

	void Menu::addWrapLayout(WrapLayout* wrapLayout) {
		wrapLayout->setSpacing(10);
		_slideRows += wrapLayout;
	}

	void Menu::addRouteButton(WrapLayout* wrapLayout, RouteMenuButton* button) {
		*wrapLayout += button;

		button->updateSelection();
	}

	void Menu::addOptionButton(WrapLayout* wrapLayout, OptionMenuButton* button) {
		*wrapLayout += button;
	}

	void Menu::updateRouteButtonsSelection() {
		for (auto button : _routeButtons) {
			button->updateSelection();
		}
	}
}