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

	class WiFiSettingsPageItem : public Button {
		public:
			explicit WiFiSettingsPageItem(const wifi_ap_record_t& accessPoint);

			wifi_ap_record_t accessPoint;

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;
			void onClick() override;
	};

	class WiFiSettingsPageDialog : public ScrollViewDialog {
		public:
			explicit WiFiSettingsPageDialog(const WiFiSettingsPageItem* item);

			TextField passwordTextField;
			Titler passwordTitle {L"Password", &passwordTextField };

			Button confirmButton {};

			bool disconnectionRequested = false;
	};

	class WiFiSettingsPage : public ScrollViewPage {
		public:
			WiFiSettingsPage();
			~WiFiSettingsPage() override;

			Switcher switcher = Switcher(L"Enable:");

			StackLayout itemsLayout {};
			Titler itemsTitle { L"Access points", &itemsLayout };

			Button updateItemButton {};

			Text updatesTitle {};
			Text updatesDescription {};
			Button updatesButton {};

			size_t WiFiScanCompletedCallbackIndex;
			size_t WiFiIsStartedCallbackIndex;
	};
}
