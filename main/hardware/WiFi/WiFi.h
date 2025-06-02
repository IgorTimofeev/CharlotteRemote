#pragma once

#include <cstring>
#include <iterator>
#include <functional>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <esp_timer.h>
#include <optional>
#include <span>

#include "esp_log.h"

#include <YOBA/main.h>

namespace pizda {
	using namespace YOBA;

	class WiFi {
		public:
			static void setup();
			static void start();
			static void stop();
			static void updateConfig();
			static void connect();
			static void disconnect();
			static void scan();

			static bool isStarted();
			static bool isConnected();
			static int getRSSI();

			static Callback<> isStartedChanged;
			static Callback<> isConnectedChanged;
			static Callback<const std::span<wifi_ap_record_t>&> scanCompleted;

		private:
			static bool _started;
			static bool _connected;

			static void setStarted(bool value);
			static void setConnected(bool value);
			static void WiFiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData);
			static void scheduleReconnection();
			static void reconnectTaskFunction(void* arg);
			static void connectUnchecked();
	};
}