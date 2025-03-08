#include "WiFiPage.h"

#include "../../theme.h"
#include "../../../constants.h"
#include "../../../hardware/WiFi.h"

namespace pizda {
	WiFiPage::WiFiPage() {
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
}