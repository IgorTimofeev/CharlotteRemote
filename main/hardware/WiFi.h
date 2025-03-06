#pragma once

#include <cstring>
#include <iterator>
#include <functional>
#include "esp_wifi.h"
#include "nvs_flash.h"
#include <esp_timer.h>
#include "esp_log.h"

namespace pizda {
	enum class WiFiState : uint8_t {
		notStarted,
		started,
		connecting,
		connected,
		disconnecting,
		disconnected
	};

	class WiFi {
		public:
			void start(const char* ssid, const char* password, wifi_auth_mode_t authMode) {
				ESP_LOGI("WiFi", "Starting");

				ESP_ERROR_CHECK(esp_netif_init());
				ESP_ERROR_CHECK(esp_event_loop_create_default());

				esp_netif_create_default_wifi_sta();

				wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
				ESP_ERROR_CHECK(esp_wifi_init(&cfg));

				esp_event_handler_instance_t eventHandlerInstanceID;

				ESP_ERROR_CHECK(esp_event_handler_instance_register(
					WIFI_EVENT,
					ESP_EVENT_ANY_ID,
					WiFiEventHandler,
					this,
					&eventHandlerInstanceID
				));

				esp_event_handler_instance_t eventHandlerInstanceIP;

				ESP_ERROR_CHECK(esp_event_handler_instance_register(
					IP_EVENT,
					IP_EVENT_STA_GOT_IP,
					WiFiEventHandler,
					this,
					&eventHandlerInstanceIP
				));

				ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

				wifi_config_t wifi_config {};

				strncpy(
					reinterpret_cast<char*>(wifi_config.sta.ssid),
					ssid,
					std::size(wifi_config.sta.ssid)
				);

				strncpy(
					reinterpret_cast<char*>(wifi_config.sta.password),
					password,
					std::size(wifi_config.sta.password)
				);

				wifi_config.sta.threshold.authmode = authMode;

				ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
				ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
				ESP_ERROR_CHECK(esp_wifi_start());
			}

			void connect() {
				ESP_LOGI("WiFi", "Connection requested");

				esp_wifi_connect();
				setState(WiFiState::connecting);
			}

			void disconnect() {
				ESP_LOGI("WiFi", "Disconnection requested");

				esp_wifi_disconnect();
				setState(WiFiState::disconnecting);
			}

			WiFiState getState() const {
				return _state;
			}

			static void WiFiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData) {
				auto instance = (WiFi*) arg;

				if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_STA_START) {
					ESP_LOGI("WiFi", "Ready to connect");

					instance->setState(WiFiState::started);
				}
				else if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_STA_DISCONNECTED) {
					ESP_LOGI("WiFi", "Disconnected");

					instance->setState(WiFiState::disconnected);
				}
				else if (eventBase == IP_EVENT && eventID == IP_EVENT_STA_GOT_IP) {
					auto event = (ip_event_got_ip_t*) eventData;
					ESP_LOGI("WiFi", "Connected, local IP is " IPSTR, IP2STR(&event->ip_info.ip));

					instance->setState(WiFiState::connected);
				}
			}

			void setOnStateChanged(const std::function<void()>& onStateChanged) {
				_onStateChanged = onStateChanged;
			}

			void scheduleConnection() {
				ESP_LOGI("WiFi", "Scheduling reconnection in %lu ms", constants::wifi::connectionInterval / 1000);

				_scheduledConnectionTime = esp_timer_get_time() + constants::wifi::connectionInterval;
			}

			void tick() {
				if (_scheduledConnectionTime == 0 || esp_timer_get_time() < _scheduledConnectionTime)
					return;

				ESP_LOGI("WiFi", "Scheduled connection time reached");

				_scheduledConnectionTime = 0;

				connect();
			}

		private:
			uint32_t _scheduledConnectionTime = 0;

			WiFiState _state = WiFiState::notStarted;

			std::function<void()> _onStateChanged;

			void setState(WiFiState value) {
				if (value == _state)
					return;

				_state = value;

				_onStateChanged();
			}
	};
}