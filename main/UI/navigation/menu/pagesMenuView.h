#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "menuView.h"
#include "pageMenuViewButton.h"
#include "UI/navigation/routes.h"
#include "resources/images.h"

namespace pizda {
	using namespace YOBA;

	class PagesMenuView : public MenuView {
		public:
			const Route* getRoute() override;
			void setRoute(const Route* route) const;
			
		protected:
			void setupButtons(PageMenuViewButton* buttons, uint8_t buttonCount, const Route** lastRoute);
			
		private:
			const Route** _lastRoute = nullptr;
			PageMenuViewButton* _buttons = nullptr;
			uint8_t _buttonCount = 0;
	};
}