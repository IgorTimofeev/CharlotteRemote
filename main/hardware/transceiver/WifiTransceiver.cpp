#include <esp_timer.h>
#include "WiFiTransceiver.h"

/* The event group allows multiple bits for each event, but we only care about two events:
* - we are connected to the AP with an IP
* - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

namespace pizda {
	uint8_t WiFiTransceiver::_retryCount = 0;
	EventGroupHandle_t WiFiTransceiver::_WiFiEventGroup = {};

	void WiFiTransceiver::setup() {
		WiFiSetup();
	}

	void WiFiTransceiver::tick() {
		if (esp_timer_get_time() < _tickInterval)
			return;


		_tickInterval = esp_timer_get_time() + constants::transceiver::tickInterval;
	}

	void WiFiTransceiver::WiFiSetup() {
		_WiFiEventGroup = xEventGroupCreate();

		ESP_ERROR_CHECK(esp_netif_init());
		ESP_ERROR_CHECK(esp_event_loop_create_default());

		esp_netif_create_default_wifi_sta();

		wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
		ESP_ERROR_CHECK(esp_wifi_init(&cfg));

		esp_event_handler_instance_t eventHandlerInstanceID;

		ESP_ERROR_CHECK(esp_event_handler_instance_register(
			WIFI_EVENT,
			ESP_EVENT_ANY_ID,
			&WiFiEventHandler,
			NULL,
			&eventHandlerInstanceID
		));

		esp_event_handler_instance_t eventHandlerInstanceIP;

		ESP_ERROR_CHECK(esp_event_handler_instance_register(
			IP_EVENT,
			IP_EVENT_STA_GOT_IP,
			&WiFiEventHandler,
			NULL,
			&eventHandlerInstanceIP
		));

		ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

		wifi_config_t wifi_config {};
		strncpy(reinterpret_cast<char*>(wifi_config.sta.ssid), constants::transceiver::wifi::ssid, sizeof(wifi_config.sta.ssid));
		strncpy(reinterpret_cast<char*>(wifi_config.sta.password), constants::transceiver::wifi::password, sizeof(wifi_config.sta.password));
		wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA_WPA2_PSK;

		ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
		ESP_ERROR_CHECK(esp_wifi_start());

		ESP_LOGI("WiFi Transceiver", "Started");

		/* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
		 * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
		EventBits_t bits = xEventGroupWaitBits(
			_WiFiEventGroup,
			WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
			pdFALSE,
			pdFALSE,
			portMAX_DELAY
		);

		/* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually happened. */
		if (bits & WIFI_CONNECTED_BIT) {
			ESP_LOGI("WiFi Transceiver", "Connected");
		}
		else if (bits & WIFI_FAIL_BIT) {
			ESP_LOGI("WiFi Transceiver", "Connection failed");
		}
		else {
			ESP_LOGE("WiFi Transceiver", "Unexpected event");
		}
	}

	void WiFiTransceiver::WiFiEventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
		if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
			esp_wifi_connect();
		}
		else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
			if (_retryCount < _maxRetries) {
				ESP_LOGI("WiFi Transceiver", "Disconnected, retrying connection");

				esp_wifi_connect();
				_retryCount++;
			}
			else {
				ESP_LOGI("WiFi Transceiver", "Disconnected, max retry count of %d exceeded", _retryCount);

				xEventGroupSetBits(_WiFiEventGroup, WIFI_FAIL_BIT);
			}
		}
		else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
			auto event = (ip_event_got_ip_t*) event_data;
			_retryCount = 0;
			xEventGroupSetBits(_WiFiEventGroup, WIFI_CONNECTED_BIT);

			ESP_LOGI("WiFi Transceiver", "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
		}
	}

}