#pragma once

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include "../../page.h"
#include "../../../elements/titler.h"
#include "../../../elements/switcher.h"

namespace pizda {
	using namespace YOBA;

	class WiFiSettingsPage : public ScrollViewPage {
		public:
			WiFiSettingsPage();

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