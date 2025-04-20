#include "menu.h"
#include "../../rc.h"
#include "main/MFD/MFDPage.h"

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
		const auto& settings = RC::getInstance().getSettings();

		// MFD
		addTitle(&_MFDTitle);

		// N/D
		_mainNDButton.setPrimaryColor(&Theme::purple);
		_mainNDButton.setChecked(settings.interface.MFDNavDisplay);

		_mainNDButton.isCheckedChanged += [this]() {
			auto& settings = RC::getInstance().getSettings();
			settings.interface.MFDNavDisplay = _mainNDButton.isChecked();
			settings.enqueueWrite();

			MFDPage::fromSettings();
		};

		addOptionButton(&_mainWL, &_mainNDButton);

		// Other
		addMFDInstrumentsModeButton(&_mainMainButton);
		addMFDInstrumentsModeButton(&_mainAutopilotButton);
		addMFDInstrumentsModeButton(&_mainPressureButton);

		addWrapLayout(&_mainWL);

		// Settings
		addTitle(&_settingsTitle);
		addRouteButton(&_settingsWL, &_mainMFDButton);
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
		button->updateSelection();

		*wrapLayout += button;
	}

	void Menu::addOptionButton(WrapLayout* wrapLayout, OptionMenuButton* button) {
		*wrapLayout += button;
	}

	void Menu::addMFDInstrumentsModeButton(MFDModeMenuButton* button) {
		button->updateSelection();

		_mainWL += button;
	}

	void Menu::updateRouteButtonsSelection() {
		for (auto button : _routeButtons) {
			button->updateSelection();
		}
	}

	void Menu::updateMFDModeButtonsSelection() {
		for (auto button : _MFDInstrumentsModeButtons) {
			button->updateSelection();
		}
	}
}