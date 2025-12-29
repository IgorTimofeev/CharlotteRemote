#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/navigation/page.h>
#include <UI/elements/titler.h>
#include <UI/elements/switcher.h>

namespace pizda {
	using namespace YOBA;

	class PersonalizationSettingsPage : public ScrollViewPage {
		public:
			PersonalizationSettingsPage();

		private:
			Switcher _LPFSwitcher { L"Enable LPF" };
			Switcher _debugOverlaySwitcher { L"Show debug overlay" };
	};
}