#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "menuView.h"
#include "pageMenuViewButton.h"
#include "../routes.h"
#include "../../../resources/images.h"

namespace pizda {
	using namespace YOBA;

	class PagesMenuView : public MenuView {
		public:
			explicit PagesMenuView(PageMenuViewButton* buttons, uint8_t buttonCount, const Route** lastRoute);

			void setup() override;

			const Route* getRoute() override;
			void setRoute(const Route* route) const;

		private:
			const Route** _lastRoute;
			PageMenuViewButton* _buttons;
			uint8_t _buttonCount;
	};
}