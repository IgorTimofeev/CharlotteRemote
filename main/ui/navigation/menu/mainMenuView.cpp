#include "mainMenuView.h"

#include "../../theme.h"
#include "../../../settings.h"
#include "../../../rc.h"
#include "../routes.h"
#include "menuRoutes.h"

namespace pizda {
	RouteMenuButton::RouteMenuButton(const Image* image, const std::wstring_view& text, const Route* route) : MenuButton(image, text), _route(route) {
		setChecked(RC::getInstance().getRoute() == route);
	}

	void RouteMenuButton::onClick() {
		Button::onClick();

		auto menuView = reinterpret_cast<MainMenuView*>(getMenuView());
		menuView->checkButton(this);

		RC::getInstance().setRoute(_route);
	}

	MainMenuView::MainMenuView() {
		title.setText(L"Settings");

		// MFD button
		MFDButton.setChecked(RC::getInstance().getRoute() == &Routes::MFD);

		MFDButton.click += [this]() {
			checkButton(&MFDButton);

			RC::getInstance().getApplication().enqueueOnTick([this]() {
				RC::getInstance().setRoute(&Routes::MFD);
				getMenu()->setView(&MenuRoutes::MFD);
			});
		};

		wrapLayout += &MFDButton;

		// Route buttons
		for (auto routeButton : routeButtons)
			wrapLayout += routeButton;
	}

	void MainMenuView::checkButton(MenuButton* button) {
		MFDButton.setChecked(button == &MFDButton);

		for (auto routeButton : routeButtons)
			routeButton->setChecked(button == routeButton);
	}
}