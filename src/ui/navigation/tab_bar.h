#pragma once

#include "Arduino.h"
#include "yoba/elements/layout.h"
#include "yoba/elements/selector.h"
#include "yoba/elements/selector_item.h"
#include "yoba/elements/image.h"
#include "yoba/animation.h"

#include "tab.h"

#include "ui/rc_application.h"

#include "yoba/elements/button.h"
#include "ui/navigation/main/main_page.h"
#include "ui/navigation/engine/engine_page.h"
#include "ui/theme.h"

using namespace yoba;

namespace ui {
	class TabBar : public Selector {
		public:
			Tab mainTab = Tab("NAV");
			MainPage mainPage = MainPage();

			Tab engineTab = Tab("ENG");
			EnginePage enginePage = EnginePage();

			TabBar() {
				addChild(&_background);

				// Menu
				const uint16_t menuSize = 40;
				_menu.setSize(Size(menuSize, Size::calculated));
				_menu.setHorizontalAlignment(Alignment::start);
				_menu.addChild(&_menuBackground);

				_menu.addChild(&_menuItemsLayout);
				setItemsLayout(&_menuItemsLayout);

				addChild(&_menu);

				// Page
				_pageLayout.setMargin(Margin(menuSize, 0, 0, 0));
				addChild(&_pageLayout);

				// Initialization
				addTabAndView(&mainTab, &mainPage);
				addTabAndView(&engineTab, &enginePage);

				setSelectedIndex(0);
			}

		protected:
			void onSelectionChanged() override {
				_pageLayout.removeChildren();

				if (getSelectedIndex() < 0)
					return;

				auto& _page = _pages[getSelectedIndex()];

				_pageLayout.addChild(_page);
			}

		private:
			Rectangle _background = Rectangle(&Theme::bg1);
			Layout _pageLayout = Layout();

			Layout _menu = Layout();
			Rectangle _menuBackground = Rectangle(&Theme::bg2);
			StackLayout _menuItemsLayout = StackLayout();

			std::vector<Page*> _pages {};

			void addTabAndView(SelectorItem* tab, Page* view) {
				_pages.push_back(view);
				addItem(tab);
			}
	};
}