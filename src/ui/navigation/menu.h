#pragma once

#include "Arduino.h"

#include "../../../lib/YOBA/src/ui/layout.h"
#include "../../../lib/YOBA/src/ui/selector.h"
#include "../../../lib/YOBA/src/ui/selectorItem.h"
#include "../../../lib/YOBA/src/animation.h"
#include "../../../lib/YOBA/src/ui/button.h"
#include "../../../lib/YOBA/src/ui/shapes/rectangle.h"
#include "../../../lib/YOBA/src/ui/stackLayout.h"

#include "menuItem.h"

#include "engine/enginePage.h"
#include "debug/debugPage.h"
#include "battery/batteryPage.h"
#include "pfd/pfdPage.h"
#include "autopilot/autopilotPage.h"
#include "radio/radioPage.h"
#include "controls/controlsPage.h"

using namespace yoba;

namespace pizdanc {
	class Menu : public Selector {
		public:
			virtual void begin();

		protected:
			void onSelectionChanged() override;

		private:
			Rectangle _background = Rectangle(&Theme::bg1);
			Layout _pageLayout = Layout();

			Layout _menu = Layout();
			Rectangle _menuBackground = Rectangle(&Theme::bg2);
			StackLayout _menuItemsLayout = StackLayout();

			std::vector<Page*> _pages {};

			// ----------------------------- Pages -----------------------------

			PFDPage _pfdPage = PFDPage();
			AutopilotPage _autopilotPage = AutopilotPage();
			ControlsPage _controlsPage = ControlsPage();
			EnginePage _enginePage = EnginePage();
			BatteryPage _batteryPage = BatteryPage();
			RadioPage _radioPage = RadioPage();
			DebugPage _debugPage = DebugPage();

			void addPage(const wchar_t* name, Page* page);
	};
}