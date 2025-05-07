#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../../theme.h"
#include "../../../settings.h"

#include "menuView.h"
#include "menuTabButton.h"
#include "menuRoutes.h"

namespace pizda {
	using namespace YOBA;

	class MenuOverlayBackground : public DitheredRectangle {
		public:
			MenuOverlayBackground();

		protected:
			void onEvent(Event* event) override;
	};

	class Menu : public RelativeStackLayout {
		public:
			explicit Menu();
			~Menu();

			void setViewRoute(const Route* route);

		private:
			static const Route* _viewRoute;

			MenuOverlayBackground _overlayBackground = {};

			Layout _slideLayout = {};
			Rectangle _slideBackground = {};

			StackLayout _tabsAndContentRows {};
			RelativeStackLayout _tabsRow {};

			MenuView* _view = nullptr;

			MenuTabButton
				MFDTab { L"MFD", &MenuRoutes::MFD },
				settingsTab {L"CONF", &MenuRoutes::settings };

			MenuTabButton* tabs[2] {
				&MFDTab,
				&settingsTab
			};
	};
}