#include "menuButton.h"

#include "menu.h"
#include "main/MFD/MFDPage.h"
#include "../../rc.h"

namespace pizda {
	// -------------------------------- Default --------------------------------

	MenuButton::MenuButton(const std::wstring_view& text) {
		setSize(Size(45, 45));
		setText(text);

		setPrimaryColor(&Theme::accent1);
	}

	void MenuButton::onRender(Renderer* renderer, const Bounds& bounds) {
		if (isChecked()) {
			// Background
			renderer->renderRectangle(bounds, 3, getPrimaryColor());
		}
		else {
			// Background
			renderer->renderRectangle(bounds, 3, &Theme::bg4);
		}

		// Text
		renderer->renderString(
			Point(
				bounds.getXCenter() - Theme::fontSmall.getWidth(getText()) / 2,
				bounds.getY2() - Theme::fontSmall.getHeight() - 2
			),
			&Theme::fontSmall,
			isChecked() ? &Theme::fg1 : &Theme::fg3,
			getText()
		);
	}

	// -------------------------------- Route --------------------------------

	RouteMenuButton::RouteMenuButton(const std::wstring_view& text, const Route* route) : MenuButton(text), _route(route) {

	}

	void RouteMenuButton::onEvent(Event* event) {
		Element::onEvent(event);

		if (event->getTypeID() != TouchDownEvent::typeID)
			return;

		auto& rc = RC::getInstance();

		rc.setRoute(_route);

		auto menu = reinterpret_cast<Menu*>(getParent()->getParent()->getParent()->getParent());
		menu->updateRouteButtonsSelection();

		rc.setMenuVisibility(false);

		event->setHandled(true);
	}

	const Route* RouteMenuButton::getRoute() const {
		return _route;
	}

	void RouteMenuButton::updateSelection() {
		setChecked(RC::getInstance().getRoute() == _route);
	}

	// -------------------------------- Option --------------------------------

	OptionMenuButton::OptionMenuButton(const std::wstring_view& text) : MenuButton(text) {
		setPrimaryColor(&Theme::fg1);
	}

	void OptionMenuButton::onEvent(Event* event) {
		Element::onEvent(event);

		if (event->getTypeID() != TouchDownEvent::typeID)
			return;

		setChecked(!isChecked());

		event->setHandled(true);
	}

	// -------------------------------- MFD --------------------------------

	MFDModeMenuButton::MFDModeMenuButton(const std::wstring_view& text, SettingsInterfaceMFDInstrumentsMode mode) : MenuButton(text), _mode(mode) {
		setPrimaryColor(&Theme::fg1);
	}

	void MFDModeMenuButton::onEvent(Event* event) {
		Element::onEvent(event);

		if (event->getTypeID() != TouchDownEvent::typeID)
			return;

		auto& rc = RC::getInstance();

		rc.getSettings().interface.MFDInstrumentsMode = _mode;
		rc.getSettings().enqueueWrite();

		auto menu = reinterpret_cast<Menu*>(getParent()->getParent()->getParent()->getParent());
		menu->updateMFDModeButtonsSelection();

		MFDPage::fromSettings();

		event->setHandled(true);
	}

	void MFDModeMenuButton::updateSelection() {
		setChecked(RC::getInstance().getSettings().interface.MFDInstrumentsMode == _mode);
	}
}