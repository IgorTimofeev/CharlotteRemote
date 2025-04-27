#pragma once

#include <cstring>
#include <iterator>
#include <functional>
#include "esp_wifi.h"
#include "nvs_flash.h"
#include <esp_timer.h>
#include "esp_log.h"

#include <YOBA/main.h>

namespace pizda {
	using namespace YOBA;

	enum class WiFiState : uint8_t {
		stopped,
		started,
		connected,
		disconnected
	};

	class WiFi {
		public:
			static void start();
			static void stop();
			static void connect();
			static void disconnect();
			static WiFiState getState();
			static void addOnStateChanged(const std::function<void(WiFiState, WiFiState)>& callback);

		private:
			static TaskHandle_t* _task;
			static WiFiState _state;
			static Callback<WiFiState, WiFiState> _onStateChanged;

			static void WiFiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData);
			static void setState(WiFiState value);
			static void reconnect(void* arg);
			static void scheduleReconnection();
	};
}