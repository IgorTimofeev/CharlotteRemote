#pragma once

#include "Arduino.h"

#include "../../../lib/yoba/src/ui.h"

#include "menuItem.h"
#include "engine/enginePage.h"
#include "debug/debugPage.h"
#include "battery/batteryPage.h"
#include "pfd/pfdPage.h"
#include "autopilot/autopilotPage.h"
#include "radio/radioPage.h"
#include "controls/controlsPage.h"

namespace pizdanc {
	class Menu : public Selector {
		public:
			void setup();

		protected:
			void onSelectionChanged() override;

		private:
			Rectangle _background = Rectangle(&Theme::bg1);
			Layout _pageLayout {};

			Layout _menu = {};
			Rectangle _menuBackground = Rectangle(&Theme::bg2);
			StackLayout _menuItemsLayout {};

			std::vector<Page*> _pages {};

			// ----------------------------- Pages -----------------------------

			PFDPage _pfdPage {};
			AutopilotPage _autopilotPage {};
			ControlsPage _controlsPage {};
			EnginePage _enginePage {};
			BatteryPage _batteryPage {};
			RadioPage _radioPage {};
			DebugPage _debugPage {};

			void addPage(const std::wstring_view& name, Page* page);
	};
}