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

			void addPage(const wchar_t* name, Page* page);
	};
}