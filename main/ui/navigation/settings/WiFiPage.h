#pragma once

#include "../../components/yoba/src/main.h"
#include "../../components/yoba/src/ui.h"

#include "../page.h"
#include "../../titler.h"
#include "../../switcher.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class WiFiPage : public ScrollViewPage {
		public:
			WiFiPage();

			Switcher switcher = Switcher(L"Enable:");

			TextField SSIDTextField;
			Titler SSIDTitle = Titler(L"SSID", &SSIDTextField);

			TextField passwordTextField;
			Titler passwordTitle = Titler(L"Password", &passwordTextField);

			Text _updatesTitle {};
			Text _updatesDescription {};
			Button _updatesButton {};
	};
}