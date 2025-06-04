#pragma once

#include <cstring>
#include <iterator>
#include <functional>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <esp_timer.h>
#include <optional>
#include <span>
#include <queue>

#include "esp_log.h"

#include <YOBA/main.h>

namespace pizda {
	using namespace YOBA;

	class WiFi {
		public:
			static void setup();
			static void start();
			static void stop();
			static void setAccessPoint(const wifi_ap_record_t& accessPoint, std::string_view password);
			static void connect();
			static void disconnect();
			static void scan();
			static bool isAccessPointSet();
			static std::optional<std::string> getAccessPointSSID();

			static bool isStarted();
			static bool isConnected();
			static int getRSSI();

			static Callback<> isStartedChanged;
			static Callback<> isConnectedChanged;
			static Callback<const std::span<wifi_ap_record_t>&> scanCompleted;

		private:
			enum class Operation : uint8_t {
				start,
				stop,
				connect,
				disconnect,
				scan
			};

			static std::deque<Operation> _operations;

			static bool _started;
			static bool _connected;

			static void setStarted(bool value);
			static void setConnected(bool value);
			static void WiFiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData);
			static void scheduleReconnection();
			static void reconnectTaskFunction(void* arg);
			static void scheduleOperation(Operation operation);
			static void finishOperation(Operation operation);
			static void runStartOperation();
			static void runStopOperation();
			static void runConnectOperation();
			static void runDisconnectOperation();
			static void runScanOperation();
			static void runOperation(Operation operation);
	};
}