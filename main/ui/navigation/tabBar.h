#pragma once

#include "../../../components/yoba/src/ui.h"
#include "../theme.h"

#include "tab.h"

namespace pizdanc {
	class TabBar : public Selector {
		public:
			explicit TabBar();

		private:
			static const uint16_t _tabsHeight = 20;

			Layout _pageLayout;

			Layout _tabsLayoutHolder;
			Rectangle _tabsBackground = Rectangle(&Theme::bg2);
			EqualStackLayout _tabsLayout = EqualStackLayout(Orientation::horizontal);
	};
}