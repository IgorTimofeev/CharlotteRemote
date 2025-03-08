#include "menu.h"
#include "../../rc.h"

namespace pizda {
	void MenuOverlayBackground::onEvent(Event* event) {
		Element::onEvent(event);

		if (event->getTypeID() != TouchDownEvent::typeID)
			return;

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

		// Title
		Theme::applyPageTitle(&_slideTitle);
		_slideTitle.setText(L"Menu");
		_slideLayout += &_slideTitle;

		// Items selector
		setItemsLayout(&_slideItemsLayout);

		addItem(&_mainTitleItem);
		addItem(&_PFDItem);
		addItem(&_NDItem);
		addItem(&_APItem);

		addItem(&_settingsTitleItem);
		addItem(&_axisItem);
		addItem(&_debugItem);

		_slideRows += &_slideItemsLayout;

		_slideLayout += &_slideRows;

		*this += &_slideLayout;
	}
}