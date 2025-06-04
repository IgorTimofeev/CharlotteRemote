#include "WiFi.h"

#include <constants.h>
#include <memory>
#include <rc.h>

namespace pizda {
	std::deque<WiFi::Operation> WiFi::_operations {};
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

		// Config
		{
			wifi_config_t config {};

			ESP_ERROR_CHECK(esp_wifi_get_config(WIFI_IF_STA, &config));

			if (!config.sta.ssid[0] != '\0') {
				strncpy(
					reinterpret_cast<char*>(config.sta.ssid),
					"IT",
					std::size(config.sta.ssid)
				);

				strncpy(
					reinterpret_cast<char*>(config.sta.password),
					"SERGTIM64ST17",
					std::size(config.sta.password)
				);

				config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

				ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &config));
			}
		}
	}

	std::optional<std::string> WiFi::getAccessPointSSID() {
		wifi_config_t config {};

		ESP_ERROR_CHECK(esp_wifi_get_config(WIFI_IF_STA, &config));

		return config.sta.ssid[0] == '\0' ? std::optional<std::string>(std::nullopt) : std::string(reinterpret_cast<const char*>(config.sta.ssid));
	}

	bool WiFi::isAccessPointSet() {
		wifi_config_t config {};

		ESP_ERROR_CHECK(esp_wifi_get_config(WIFI_IF_STA, &config));

		ESP_LOGI("WiFi", "Checking if ssid == nullchar: %d", config.sta.ssid[0] == '\0');

		return config.sta.ssid[0] != '\0';
	}

	void WiFi::start() {
		if (isStarted())
			return;

		scheduleOperation(Operation::start);
	}

	void WiFi::stop() {
		if (!isStarted())
			return;

		scheduleOperation(Operation::stop);
	}

	void WiFi::setAccessPoint(const wifi_ap_record_t& accessPoint, std::string_view password) {
		ESP_LOGI("WiFi", "Setting access point %s with password %s", reinterpret_cast<const char*>(accessPoint.ssid), password.data());

		wifi_config_t config {};

		strncpy(
			reinterpret_cast<char*>(config.sta.ssid),
			reinterpret_cast<const char*>(accessPoint.ssid),
			std::size(config.sta.ssid)
		);

		strncpy(
			reinterpret_cast<char*>(config.sta.password),
			password.data(),
			std::size(config.sta.password)
		);

		config.sta.threshold.authmode = accessPoint.authmode;

		ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &config));
	}

	void WiFi::connect() {
		if (!isStarted() || isConnected())
			return;

		scheduleOperation(Operation::connect);
	}

	void WiFi::disconnect() {
		if (!isStarted() || !isConnected())
			return;

		scheduleOperation(Operation::disconnect);
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

			setStarted(true);
			finishOperation(Operation::start);
		}
		else if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_STA_STOP) {
			ESP_LOGI("WiFi", "Stopped");

			setStarted(false);
			finishOperation(Operation::stop);
		}
		else if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_STA_DISCONNECTED) {
			const auto event = static_cast<wifi_event_sta_disconnected_t*>(eventData);

			ESP_LOGI("WiFi", "Disconnected, reason code: %d", event->reason);

			setConnected(false);
			finishOperation(Operation::disconnect);
		}
		else if (eventBase == IP_EVENT && eventID == IP_EVENT_STA_GOT_IP) {
//			auto event = (ip_event_got_ip_t*) eventData;
			ESP_LOGI("WiFi", "Connected");

			setConnected(true);
			finishOperation(Operation::connect);
		}
		else if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_SCAN_DONE) {
			const auto event = static_cast<wifi_event_sta_scan_done_t*>(eventData);

			// Count
			uint16_t accessPointCount = event->number;
			ESP_LOGI("WiFi", "Scan finished, event %d", accessPointCount);

			// List
			const auto accessPoints = std::make_unique<wifi_ap_record_t[]>(accessPointCount);
			const auto state = esp_wifi_scan_get_ap_records(&accessPointCount, accessPoints.get());

			ESP_LOGI("WiFi", "esp_wifi_scan_get_ap_records count: %d", accessPointCount);

			if (state == ESP_OK) {
				scanCompleted(std::span(accessPoints.get(), accessPointCount));
			}
			else {
				ESP_ERROR_CHECK_WITHOUT_ABORT(state);
			}

			esp_wifi_clear_ap_list();
			finishOperation(Operation::scan);
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
		scheduleOperation(Operation::scan);
	}

	void WiFi::reconnectTaskFunction(void* arg) {
		ESP_LOGI("WiFi", "Scheduling reconnection in %lu ms", constants::wifi::connectionInterval / 1000);

		vTaskDelay(pdMS_TO_TICKS(constants::wifi::connectionInterval / 1000));

		ESP_LOGI("WiFi", "Scheduled reconnection time reached");

		connect();

		vTaskDelete(nullptr);
	}

	void WiFi::scheduleOperation(Operation operation) {
		_operations.push_back(operation);

		if (_operations.size() == 1)
			runOperation(operation);
	}

	void WiFi::finishOperation(Operation operation) {
		if (_operations.front() == operation) {
			_operations.pop_front();

			if (_operations.empty())
				return;
		}

		runOperation(_operations.front());
	}

	void WiFi::runStartOperation() {
		ESP_LOGI("WiFi", "Start requested");

		ESP_ERROR_CHECK(esp_wifi_start());
	}

	void WiFi::runStopOperation() {
		ESP_LOGI("WiFi", "Stop requested");

		esp_wifi_stop();
	}

	void WiFi::runConnectOperation() {
		ESP_LOGI("WiFi", "Connect requested");

		if (!isAccessPointSet()) {
			ESP_LOGI("WiFi", "Couldn't connect, because SSID & password haven't been configured yet");
			return;
		}

		esp_wifi_connect();
	}

	void WiFi::runDisconnectOperation() {
		ESP_LOGI("WiFi", "Disconnect requested");

		esp_wifi_disconnect();
	}

	void WiFi::runScanOperation() {
		ESP_LOGI("WiFi", "Scan started");

		wifi_scan_config_t config {};
		config.scan_type = WIFI_SCAN_TYPE_ACTIVE;
		config.show_hidden = false;

		esp_wifi_scan_start(&config, false);
	}

	void WiFi::runOperation(Operation operation) {
		switch (operation) {
			case Operation::start:
				runStartOperation();
				break;
			case Operation::stop:
				runStopOperation();
				break;
			case Operation::connect:
				runConnectOperation();
				break;
			case Operation::disconnect:
				runDisconnectOperation();
				break;
			case Operation::scan:
				runScanOperation();
				break;
		}
	}
}
