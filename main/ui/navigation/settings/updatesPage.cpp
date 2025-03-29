#include "updatesPage.h"

#include "../../../hardware/WiFi.h"
#include "../../../hardware/OTA.h"

#include "../../../constants.h"
#include "../../../rc.h"
#include "../../dialog.h"

namespace pizda {
	UpdatesPage::UpdatesPage() {
		// Title
		title.setText(L"Updates");

		// Description
		Theme::applyDescription(&_description);
		_description.setWrappingEnabled(true);
		_description.setText(L"If you're connected to the Internet via Wi-Fi, you can update device's firmware directly from our servers and continue to maintain your cable virginity");
		rows += &_description;

		// Button
		Theme::apply(&_button);
		_button.setText(L"Begin");

		_button.isCheckedChanged += [this]() {
			if (_button.isChecked())
				return;

			auto& rc = RC::getInstance();

			auto dialog = new ProgressDialog();
			dialog->title.setText(L"Updating firmware");
			dialog->description.setText(L"Do not turn off your device and wait for the process to finish. Otherwise, you risk getting a fucking awesome but poorly functional brick.");
			dialog->show(&rc.getApplication());

			auto ota = new OTA();

			ota->onProgressChanged += [dialog](uint16_t progress) {
				dialog->setProgress(progress);
			};

			ota->onFinished += []() {
				esp_restart();
			};

			ota->start();
		};

		rows += &_button;
	}
}