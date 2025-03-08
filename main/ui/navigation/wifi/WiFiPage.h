#pragma once

#include "../../../../components/yoba/src/main.h"
#include "../../../../components/yoba/src/ui.h"

#include "../page.h"
#include "../../theme.h"
#include "../../titler.h"
#include "../../switcher.h"
#include "../../../constants.h"
#include "../../../hardware/WiFi.h"

namespace pizda {
	using namespace yoba;
	using namespace yoba::ui;

	class WiFiPage : public ScrollViewPage {
		public:
			WiFiPage() {
				// Title
				title.setText(L"Wi-Fi");

				// State
				switcher.getSwitch().setChecked(true);
				rows += &switcher;

				// SSID
				Theme::apply(&SSIDTextField);
				SSIDTextField.setText(L"IT");

				rows += &SSIDTitle;

				// Password
				Theme::apply(&passwordTextField);
				passwordTextField.setText(L"SERGTIM64ST17");
				passwordTextField.setMask(L'*');

				rows += &passwordTitle;
			}

			Switcher switcher = Switcher(L"Enable:");

			TextField SSIDTextField;
			Titler SSIDTitle = Titler(L"SSID", &SSIDTextField);

			TextField passwordTextField;
			Titler passwordTitle = Titler(L"Password", &passwordTextField);
	};
}