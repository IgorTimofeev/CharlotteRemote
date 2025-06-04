#pragma once

#include <esp_wifi.h>
#include <functional>
#include <hardware/WiFi/WiFi.h>

#include <YOBA/main.h>
#include <YOBA/UI.h>

#include <UI/navigation/page.h>
#include <UI/elements/titler.h>
#include <UI/elements/switcher.h>
#include <UI/elements/dialogs/dialog.h>
#include <UI/elements/dialogs/scrollViewDialog.h>
#include <hardware/WiFi/WiFi.h>
#include <utils/string.h>

namespace pizda {
	using namespace YOBA;

	class WiFiSettingsPageAccessPointItem : public Button {
		public:
			explicit WiFiSettingsPageAccessPointItem(const wifi_ap_record_t& accessPoint);

			wifi_ap_record_t accessPoint;

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;
			void onClick() override;
	};

	class WiFiSettingsPageDialog : public ScrollViewDialog {
		public:
			explicit WiFiSettingsPageDialog(const WiFiSettingsPageAccessPointItem* item);

			TextField passwordTextField;
			Titler passwordTitle {L"Password", &passwordTextField };

			Button confirmButton {};

			bool disconnectionRequested = false;
	};

	class WiFiSettingsPage : public ScrollViewPage {
		public:
			WiFiSettingsPage();
			~WiFiSettingsPage() override;

			Switcher switcher = Switcher(L"Enabled");

			StackLayout accessPointsLayout {};
			Titler accessPointsTitle { L"Available networks", &accessPointsLayout };

			Button updateAccessPointsButton {};

			Text updatesTitle {};
			Text updatesDescription {};
			Button updatesButton {};

			size_t WiFiScanCompletedCallbackIndex;
			size_t WiFiIsStartedCallbackIndex;
	};
}
