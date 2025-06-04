#include "WiFiPage.h"

#include <UI/theme.h>
#include <constants.h>
#include <rc.h>
#include <UI/elements/dialogs/progressDialog.h>
#include <utils/string.h>

namespace pizda {
	WiFiSettingsPageAccessPointItem::WiFiSettingsPageAccessPointItem(const wifi_ap_record_t& accessPoint) : accessPoint(accessPoint) {
		setHeight(Theme::elementHeight);

		setText(StringUtils::toWString(reinterpret_cast<const char*>(&this->accessPoint.ssid)));
	}

	void WiFiSettingsPageAccessPointItem::onRender(Renderer* renderer, const Bounds& bounds) {
		renderer->renderFilledRectangle(bounds, Theme::cornerRadius, &Theme::bg3);
		renderer->renderRectangle(bounds, Theme::cornerRadius, isActive() ? &Theme::fg1 : &Theme::bg3);

		const auto centerY = bounds.getYCenter();

		// RSSI
		constexpr static uint8_t rssiRadiusStep = 3;
		constexpr static uint8_t rssiRadius = rssiRadiusStep * 3;
		constexpr static uint8_t rssiAngleFrom = 30;
		constexpr static uint8_t rssiAngleTo = 150;

		const auto rssiPosition = Point(bounds.getX() + 8 + rssiRadius, centerY + 5);
		renderer->renderPixel(rssiPosition, &Theme::accent1);
		renderer->renderArc(rssiPosition, rssiRadiusStep    , rssiAngleFrom * 255 / 360, rssiAngleTo * 255 / 360, accessPoint.rssi > -80 ? &Theme::accent1 : &Theme::fg5);
		renderer->renderArc(rssiPosition, rssiRadiusStep * 2, rssiAngleFrom * 255 / 360, rssiAngleTo * 255 / 360, accessPoint.rssi > -60 ? &Theme::accent1 : &Theme::fg5);
		renderer->renderArc(rssiPosition, rssiRadiusStep * 3, rssiAngleFrom * 255 / 360, rssiAngleTo * 255 / 360, accessPoint.rssi > -40 ? &Theme::accent1 : &Theme::fg5);

		// SSID
		renderer->renderString(
			Point(
				rssiPosition.getX() + rssiRadius + 10,
				centerY - Theme::fontNormal.getHeight() / 2
			),
			&Theme::fontNormal,
			&Theme::fg1,
			getText()
		);

		// RSSI
	}

	void WiFiSettingsPageAccessPointItem::onClick() {
		(new WiFiSettingsPageDialog(this))->show();
	}

	WiFiSettingsPageDialog::WiFiSettingsPageDialog(const WiFiSettingsPageAccessPointItem* item) {
		title.setText(item->getText());

		// Password
		Theme::apply(&passwordTextField);
		passwordTextField.setText(L"SERGTIM64ST17");
		// passwordTextField.setMask(L'*');

		rows += &passwordTitle;

		// Confirm
		Theme::applyPrimary(&confirmButton);
		confirmButton.setText(L"Connect");

		confirmButton.click += [this, item] {
			auto& rc = RC::getInstance();

			rc.getApplication().scheduleOnTick([this, &rc, item] {
				auto& settings = rc.getSettings();
				settings.WiFi.scheduleWrite();

				WiFi::disconnect();
				WiFi::setAccessPoint(item->accessPoint, StringUtils::toString(passwordTextField.getText()));
				WiFi::connect();

				hide();
				delete this;
			});
		};

		rows += &confirmButton;
	}

	WiFiSettingsPage::WiFiSettingsPage() {
		auto& rc = RC::getInstance();
		auto& settings = rc.getSettings();

		// Title
		title.setText(L"Wi-Fi");

		// State
		switcher.getSwitch().setActive(true);

		switcher.getSwitch().isActiveChanged += [this, &settings] {
			settings.WiFi.enabled = switcher.getSwitch().isActive();
			settings.WiFi.scheduleWrite();

			if (settings.WiFi.enabled) {
				WiFi::start();
			}
			else {
				accessPointsLayout.removeAndDeleteChildren();
				accessPointsTitle.setVisible(false);

				WiFi::stop();
			}
		};

		switcher.getSwitch().setActive(settings.WiFi.enabled);

		rows += &switcher;

		// Items
		accessPointsLayout.setSpacing(5);
		rows += &accessPointsTitle;

		Theme::applySecondary(&updateAccessPointsButton);
		updateAccessPointsButton.setText(L"Refresh list");

		updateAccessPointsButton.click += [this] {
			if (WiFi::isStarted()) {
				WiFi::scan();
			}
		};

		rows += &updateAccessPointsButton;

		// Updates

		// Title
		Theme::applyPageTitle(&updatesTitle);
		updatesTitle.setText(L"Updates");
		rows += &updatesTitle;

		// Description
		Theme::applyDescription(&updatesDescription);
		updatesDescription.setWrappingEnabled(true);
		updatesDescription.setText(L"You can update device's firmware directly from our servers and continue to maintain your cable virginity");
		rows += &updatesDescription;

		// Button
		Theme::applyPrimary(&updatesButton);
		updatesButton.setText(L"Begin");

		updatesButton.click += [] {
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

		rows += &updatesButton;

		// Initialization
		WiFiIsStartedCallbackIndex = WiFi::isStartedChanged += [this] {
			if (WiFi::isStarted()) {
				WiFi::scan();
			}
		};

		WiFiScanCompletedCallbackIndex = WiFi::scanCompleted += [this](const std::span<wifi_ap_record_t>& accessPoints) {
			accessPointsLayout.removeAndDeleteChildren();

			for (const auto& accessPoint : accessPoints) {
				accessPointsLayout += new WiFiSettingsPageAccessPointItem(accessPoint);
			}

			accessPointsTitle.setVisible(accessPoints.size() > 0);
		};

		if (WiFi::isStarted()) {
			WiFi::scan();
		}
	}

	WiFiSettingsPage::~WiFiSettingsPage() {
		WiFi::isStartedChanged -= WiFiIsStartedCallbackIndex;
		WiFi::scanCompleted -= WiFiScanCompletedCallbackIndex;
	}
}
