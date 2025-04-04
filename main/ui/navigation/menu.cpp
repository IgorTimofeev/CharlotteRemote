#include "menu.h"
#include "../../rc.h"

namespace pizda {
	void MenuOverlayBackground::onEvent(Event* event) {
		Element::onEvent(event);

		if (!ScreenEvent::isScreen(event))
			return;

		event->setHandled(true);

		RC::getInstance().setMenuVisibility(false);
	}

	Menu::Menu() {
		*this += &overlayBackground;

		// Holder
		_slideLayout.setWidth(_menuWidth);
		_slideLayout.setHorizontalAlignment(Alignment::start);

		// Items background
		_slideBackground.setFillColor(&Theme::bg2);
		_slideLayout += &_slideBackground;

		// Items selector
		addTitleItem(&_mainTitleItem);
		addPageItem(&_mainMFDItem);
		addPageItem(&_mainNDItem);

		addTitleItem(&_settingsTitleItem);
		addPageItem(&_settingsWiFiItem);
		addPageItem(&_settingsAxisItem);
		addPageItem(&_settingsUpdatesItem);

		addTitleItem(&_developerTitleItem);
		addPageItem(&_developerSpeakerItem);
		addPageItem(&_developerUITestItem);

		_slideItemsLayout.setMargin(Margin(0, 5, 0, 0));
		_slideLayout += &_slideItemsLayout;

		*this += &_slideLayout;
	}

	void Menu::addItem(MenuItem* item) {
		_slideItemsLayout += item;
	}

	void Menu::addTitleItem(TitleMenuItem* item) {
		addItem(item);
	}

	void Menu::addPageItem(PageMenuItem* item) {
		item->updateSelection();

		addItem(item);
	}

	void Menu::updatePageItemsSelection() {
		for (auto element : _slideItemsLayout) {
			if (!element->isEnabled())
				continue;

			dynamic_cast<PageMenuItem*>(element)->updateSelection();
		}
	}
}