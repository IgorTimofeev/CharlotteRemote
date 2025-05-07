#include "routeMenuButton.h"

#include "../../../rc.h"

namespace pizda {
	RouteMenuButton::RouteMenuButton(const Image* image, const std::wstring_view& text, const Route* mainRoute, const Route* upperSectionRoute) :
		MenuButton(image, text),
		_mainRoute(mainRoute),
		_upperSectionRoute(upperSectionRoute)
	{
		setChecked(RC::getInstance().getRoute() == _mainRoute);
	}

	void RouteMenuButton::onClick() {
		Button::onClick();

		RC::getInstance().getApplication().enqueueOnTick([this]() {
			getMenuSection()->getMenu()->setRoute(_mainRoute);
		});
	}

	const Route* RouteMenuButton::getMainRoute() const {
		return _mainRoute;
	}

	const Route* RouteMenuButton::getUpperSectionRoute() const {
		return _upperSectionRoute;
	}
}