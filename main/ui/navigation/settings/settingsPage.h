#pragma once

#include "../../../components/yoba/src/ui.h"
#include "../page.h"
#include "../../theme.h"
#include "settingsPageTab.h"
#include "debug/debugPage.h"
#include "radio/radioPage.h"
#include "battery/batteryPage.h"
#include "controls/controlsPage.h"

namespace pizdanc {
	class SettingsPage : public Page {
		public:
			explicit SettingsPage();

			~SettingsPage() override;

		private:
			static const uint16_t tabBarSize = 30;
			static uint8_t _tabIndex;

			Selector _tabBar;

			Layout _pageLayout;

			Layout _tabsLayoutHolder;
			Rectangle _tabsBackground = Rectangle(&Theme::bg2);
			EqualStackLayout _tabsLayout = EqualStackLayout(Orientation::horizontal);

			SettingsPageTab
				_debugTab = SettingsPageTab(L"DBG", []() {
					return new DebugPage();
				}),

				_radioTab = SettingsPageTab(L"COM", []() {
					return new RadioPage();
				}),

				_controlsTab = SettingsPageTab(L"CTL", []() {
					return new ControlsPage();
				}),

				_batteryTab = SettingsPageTab(L"BAT", []() {
					return new BatteryPage();
				});
	};
}