#pragma once

#include "../../../components/yoba/src/ui.h"
#include "../page.h"
#include "../../theme.h"

namespace pizdanc {
	class SettingsPage : public Page {
		public:
			explicit SettingsPage();

			~SettingsPage();

		private:
			static const uint16_t tabBarWidth = 40;

			Selector _tabBar;

			Layout _pageLayout;

			Layout _tabsLayoutHolder;
			Rectangle _tabsBackground = Rectangle(&Theme::bg2);
			EqualStackLayout _tabsLayout = EqualStackLayout(Orientation::horizontal);
	};
}