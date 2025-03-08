#include "WiFi.h"
#include "../constants.h"

namespace pizda {
	void WiFi::start() {
		if (getState() != WiFiState::stopped)
			return;

		ESP_LOGI("WiFi", "Starting requested");

		ESP_ERROR_CHECK(esp_netif_init());

		auto state = esp_event_loop_create_default();
		assert(state == ESP_OK || state == ESP_ERR_INVALID_STATE);

		esp_netif_create_default_wifi_sta();

		wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
		ESP_ERROR_CHECK(esp_wifi_init(&cfg));

		esp_event_handler_instance_t eventHandlerInstanceID;

		ESP_ERROR_CHECK(esp_event_handler_instance_register(
			WIFI_EVENT,
			ESP_EVENT_ANY_ID,
			WiFiEventHandler,
			nullptr,
			&eventHandlerInstanceID
		));

		esp_event_handler_instance_t eventHandlerInstanceIP;

		ESP_ERROR_CHECK(esp_event_handler_instance_register(
			IP_EVENT,
			IP_EVENT_STA_GOT_IP,
			WiFiEventHandler,
			nullptr,
			&eventHandlerInstanceIP
		));

		ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

		wifi_config_t wifi_config {};

		strncpy(
			reinterpret_cast<char*>(wifi_config.sta.ssid),
			constants::wifi::ssid,
			std::size(wifi_config.sta.ssid)
		);

		strncpy(
			reinterpret_cast<char*>(wifi_config.sta.password),
			constants::wifi::password,
			std::size(wifi_config.sta.password)
		);

		wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

		ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
		ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
		ESP_ERROR_CHECK(esp_wifi_start());
	}

	void WiFi::stop() {
		if (_state == WiFiState::stopped)
			return;

		ESP_LOGI("WiFi", "Stopping requested");

		esp_wifi_stop();
	}

	void WiFi::connect() {
		if (_state == WiFiState::stopped || _state == WiFiState::connected)
			return;

		ESP_LOGI("WiFi", "Connection requested");

		esp_wifi_connect();
	}

	void WiFi::disconnect() {
		if (_state == WiFiState::stopped || _state == WiFiState::connected)
			return;

		ESP_LOGI("WiFi", "Disconnection requested");

		esp_wifi_disconnect();
	}

	WiFiState WiFi::getState() {
		return _state;
	}

	void WiFi::WiFiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData) {
		if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_STA_START) {
			ESP_LOGI("WiFi", "Ready to connect");

			setState(WiFiState::started);
			connect();
		}
		else if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_STA_STOP) {
			ESP_LOGI("WiFi", "Stopped");

			setState(WiFiState::stopped);
		}
		else if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_STA_DISCONNECTED) {
			ESP_LOGI("WiFi", "Disconnected");

			setState(WiFiState::disconnected);

			scheduleReconnection();
		}
		else if (eventBase == IP_EVENT && eventID == IP_EVENT_STA_GOT_IP) {
			auto event = (ip_event_got_ip_t*) eventData;
			ESP_LOGI("WiFi", "Connected, local IP is " IPSTR, IP2STR(&event->ip_info.ip));

			setState(WiFiState::connected);
		}
	}

	void WiFi::addOnStateChanged(const std::function<void(WiFiState, WiFiState)>& callback) {
		_onStateChanged += callback;
	}

	void WiFi::reconnect(void* args) {
		ESP_LOGI("WiFi", "Scheduling reconnection in %lu ms", constants::wifi::connectionInterval / 1000);

		vTaskDelay(constants::wifi::connectionInterval / 1000 / portTICK_PERIOD_MS);

		ESP_LOGI("WiFi", "Scheduled reconnection time reached");

		connect();

		vTaskDelete(NULL);
	}

	void WiFi::scheduleReconnection() {
		xTaskCreate(reconnect, "WiFi reconnection task", 1024, NULL, 3, NULL);
	}

	TaskHandle_t* WiFi::_task = nullptr;
	WiFiState WiFi::_state = WiFiState::stopped;
	Callback<WiFiState, WiFiState> WiFi::_onStateChanged = {};

	void WiFi::setState(WiFiState value) {
		if (value == _state)
			return;

		const auto fromState = _state;
		_state = value;

		_onStateChanged(fromState, _state);
	}
}