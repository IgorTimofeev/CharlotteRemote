#pragma once

#include "Arduino.h"

#include "../../../lib/YOBA/src/ui/container.h"
#include "../../../lib/YOBA/src/ui/selector.h"
#include "../../../lib/YOBA/src/animation.h"
#include "../../../lib/YOBA/src/ui/button.h"
#include "../../../lib/YOBA/src/ui/rectangle.h"
#include "../../../lib/YOBA/src/ui/stackContainer.h"

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
			Container _pageLayout {};

			Container _menu = {};
			Rectangle _menuBackground = Rectangle(&Theme::bg2);
			StackContainer _menuItemsLayout {};

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