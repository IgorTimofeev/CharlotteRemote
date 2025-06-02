#include "WiFi.h"

#include <constants.h>
#include <memory>
#include <rc.h>

namespace pizda {
	bool WiFi::_connected = false;
	bool WiFi::_started = false;

	Callback<> WiFi::isStartedChanged {};
	Callback<> WiFi::isConnectedChanged {};
	Callback<const std::span<wifi_ap_record_t>&> WiFi::scanCompleted {};

	void WiFi::setup() {
		ESP_ERROR_CHECK(esp_netif_init());

		const auto state = esp_event_loop_create_default();
		assert(state == ESP_OK || state == ESP_ERR_INVALID_STATE);

		esp_netif_create_default_wifi_sta();

		const wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
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
		ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
	}

	void WiFi::start() {
		if (isStarted())
			return;

		ESP_LOGI("WiFi", "Start requested");

		ESP_ERROR_CHECK(esp_wifi_start());
	}

	void WiFi::stop() {
		if (!isStarted())
			return;

		ESP_LOGI("WiFi", "Stop requested");

		esp_wifi_stop();
	}

	void WiFi::updateConfig() {
		const auto& settings = RC::getInstance().getSettings();

		if (!settings.WiFi.current.has_value()) {
			ESP_LOGI("WiFi", "Couldn't update config, because SSID & password haven't been configured yet");
			return;
		}

		wifi_config_t wifi_config {};

		strncpy(
			reinterpret_cast<char*>(wifi_config.sta.ssid),
			settings.WiFi.current.value().ssid.data(),
			std::size(wifi_config.sta.ssid)
		);

		strncpy(
			reinterpret_cast<char*>(wifi_config.sta.password),
			settings.WiFi.current.value().password.data(),
			std::size(wifi_config.sta.password)
		);

		wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

		ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
	}

	void WiFi::connect() {
		if (!isStarted() || isConnected())
			return;

		connectUnchecked();
	}

	void WiFi::connectUnchecked() {
		const auto& settings = RC::getInstance().getSettings();

		if (!settings.WiFi.current.has_value()) {
			ESP_LOGI("WiFi", "Couldn't connect, because SSID & password haven't been configured yet");
			return;
		}

		esp_wifi_connect();
	}

	void WiFi::disconnect() {
		if (!isStarted() || !isConnected())
			return;

		ESP_LOGI("WiFi", "Disconnect requested");

		esp_wifi_disconnect();
	}

	bool WiFi::isStarted() {
		return _started;
	}

	bool WiFi::isConnected() {
		return _connected;
	}

	void WiFi::setStarted(bool value) {
		if (value == _started)
			return;

		_started = value;

		ESP_LOGI("WiFi", "IsStarted changed: %d", _started);

		isStartedChanged();
	}

	void WiFi::setConnected(bool value) {
		if (value == _connected)
			return;

		_connected = value;

		ESP_LOGI("WiFi", "IsConnected changed: %d", _connected);

		isConnectedChanged();
	}

	void WiFi::WiFiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData) {
		if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_STA_START) {
			ESP_LOGI("WiFi", "Started");

			connectUnchecked();
			setStarted(true);
		}
		else if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_STA_STOP) {
			ESP_LOGI("WiFi", "Stopped");

			setStarted(false);
		}
		else if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_STA_DISCONNECTED) {
			const auto event = static_cast<wifi_event_sta_disconnected_t*>(eventData);

			ESP_LOGI("WiFi", "Disconnected, reason code: %d", event->reason);

			setConnected(false);
			// connect();
		}
		else if (eventBase == IP_EVENT && eventID == IP_EVENT_STA_GOT_IP) {
//			auto event = (ip_event_got_ip_t*) eventData;
			ESP_LOGI("WiFi", "Connected");

			setConnected(true);
		}
		else if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_SCAN_DONE) {
			const auto event = static_cast<wifi_event_sta_scan_done_t*>(eventData);

			ESP_LOGI("WiFi", "Scan finished, event %d", event->number);

			// Count
			uint16_t accessPointCount = 0;
			auto state = esp_wifi_scan_get_ap_num(&accessPointCount);

			ESP_LOGI("WiFi", "Found AP count: %d", accessPointCount);

			if (state != ESP_OK)
				ESP_ERROR_CHECK_WITHOUT_ABORT(state);

			// List
			const auto accessPoints = std::make_unique<wifi_ap_record_t[]>(accessPointCount);
			state = esp_wifi_scan_get_ap_records(&accessPointCount, accessPoints.get());

			ESP_LOGI("WiFi", "esp_wifi_scan_get_ap_records count: %d", accessPointCount);

			if (state == ESP_OK) {
				scanCompleted(std::span(accessPoints.get(), accessPointCount));
			}
			else {
				ESP_ERROR_CHECK_WITHOUT_ABORT(state);
			}

			esp_wifi_clear_ap_list();
		}
	}

	void WiFi::scheduleReconnection() {
		xTaskCreate(reconnectTaskFunction, "WiFi reconnection task", 4096, nullptr, 3, nullptr);
	}

	int WiFi::getRSSI() {
		int result = 0;

		if (esp_wifi_sta_get_rssi(&result) != ESP_OK)
			result = 0;

		return result;
	}

	void WiFi::scan() {
		ESP_LOGI("WiFi", "Scan started");

		wifi_scan_config_t config {};
		config.scan_type = WIFI_SCAN_TYPE_ACTIVE;
		config.show_hidden = false;

		esp_wifi_scan_start(&config, false);
	}

	void WiFi::reconnectTaskFunction(void* arg) {
		ESP_LOGI("WiFi", "Scheduling reconnection in %lu ms", constants::wifi::connectionInterval / 1000);

		vTaskDelay(pdMS_TO_TICKS(constants::wifi::connectionInterval / 1000));

		ESP_LOGI("WiFi", "Scheduled reconnection time reached");

		connect();

		vTaskDelete(nullptr);
	}
}
