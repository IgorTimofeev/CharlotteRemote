#include "pageMenuViewButton.h"

#include "pagesMenuView.h"

namespace pizda {
	PageMenuViewButton::PageMenuViewButton(const Image* image, const std::wstring_view& text, const Route* route) :
		MenuViewButton(image, text),
		_route(route)
	{

	}

	void PageMenuViewButton::onClick() {
		Button::onClick();

		reinterpret_cast<PagesMenuView*>(getMenuView())->setRoute(_route);
	}

	const Route* PageMenuViewButton::getRoute() const {
		return _route;
	}
}