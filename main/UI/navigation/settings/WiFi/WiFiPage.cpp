#include "WiFiSettingsPage.h"

#include "../../../theme.h"
#include "../../../../constants.h"
#include "../../../../hardware/WiFi/WiFi.h"
//#include "../../../../hardware/OTA.h"
#include "../../../../rc.h"
#include "../../../elements/dialogs/progressDialog.h"

namespace pizda {
	WiFiSettingsPage::WiFiSettingsPage() {
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

		// Updates

		// Title
		Theme::applyTitle(&_updatesTitle);
		_updatesTitle.setText(L"Updates");
		rows += &_updatesTitle;

		// Description
		Theme::applyDescription(&_updatesDescription);
		_updatesDescription.setWrappingEnabled(true);
		_updatesDescription.setText(L"If you're connected to the Internet via Wi-Fi, you can update device's firmware directly from our servers and continue to maintain your cable virginity");
		rows += &_updatesDescription;

		// Button
		Theme::apply(&_updatesButton);
		_updatesButton.setText(L"Begin");

		_updatesButton.click += [] {
			auto& rc = RC::getInstance();

			const auto dialog = new ProgressDialog();
			dialog->title.setText(L"Updating firmware");
			dialog->descriptionText.setText(L"Do not turn off your device and wait for the process to finish. Otherwise, you risk getting a fucking awesome but poorly functional brick.");
			dialog->show();

//			auto ota = new OTA();
//
//			ota->onProgressChanged += [dialog](uint16_t progress) {
//				dialog->setProgress(progress);
//			};
//
//			ota->onFinished += []() {
//				esp_restart();
//			};
//
//			ota->start();
		};

		rows += &_updatesButton;
	}
}