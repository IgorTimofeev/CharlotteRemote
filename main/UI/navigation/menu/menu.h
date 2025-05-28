#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/theme.h>
#include <UI/elements/dialogs/dialog.h>
#include <settings/settings.h>

#include "menuView.h"
#include "menuTabButton.h"
#include "menuRoutes.h"

namespace pizda {
	using namespace YOBA;

	class Menu : public Dialog {
		public:
			explicit Menu();
			~Menu() override;

			void setViewRoute(const Route* route);

		private:
			static const Route* _viewRoute;

			StackLayout _tabsAndContentRows {};
			RelativeStackLayout _tabsRow {};

			MenuView* _view = nullptr;

			MenuTabButton tabs[3] {
				{ L"MFD", &MenuRoutes::MFD },
				{ L"NAV", &MenuRoutes::nav },
				{ L"CONF", &MenuRoutes::settings },
			};
	};
}