#include "WiFiTransceiver.h"
#include "../../rc.h"
#include <esp_timer.h>

namespace pizda {
	void WiFiTransceiver::setup() {
		WiFiSetup();

	}

	void WiFiTransceiver::tick() {
		if (esp_timer_get_time() < _tickInterval)
			return;

		if (!_WiFiConnected)
			return;

		if (_tcp.isConnected()) {
			_tcp.send();

			if (_tcp.isReadyToSendNext()) {
				fillRemotePacket();

				_tcp.setSendingBuffer((uint8_t*) &_remotePacket, sizeof(RemotePacket));
			}

			_tcp.receive();

			if (_tcp.isReadyToReceiveNext()) {
				if (_tcp.isReceivingFinished()) {
					RC::getInstance().handleAircraftPacket(&_aircraftPacket);
				}

				_tcp.setReceivingBuffer((uint8_t*) &_aircraftPacket, sizeof(AircraftPacket));
			}
		}
		else {
			_tcp.connect();
		}

		_tickInterval = esp_timer_get_time() + constants::transceiver::tickInterval;
	}

	void WiFiTransceiver::WiFiSetup() {
		ESP_LOGI("Transceiver (WiFi)", "Starting");

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
			constants::transceiver::wifi::ssid,
			std::size(wifi_config.sta.ssid)
		);

		strncpy(
			reinterpret_cast<char*>(wifi_config.sta.password),
			constants::transceiver::wifi::password,
			std::size(wifi_config.sta.password)
		);

		wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA_WPA2_PSK;

		ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
		ESP_ERROR_CHECK(esp_wifi_start());
	}

	void WiFiTransceiver::WiFiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData) {
		auto instance = (WiFiTransceiver*) arg;

		if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_STA_START) {
			ESP_LOGI("Transceiver (WiFi)", "Connecting");

			esp_wifi_connect();
		}
		else if (eventBase == WIFI_EVENT && eventID == WIFI_EVENT_STA_DISCONNECTED) {
			ESP_LOGI("Transceiver (WiFi)", "Disconnected, retrying connection");

			instance->_WiFiConnected = false;
			esp_wifi_connect();
		}
		else if (eventBase == IP_EVENT && eventID == IP_EVENT_STA_GOT_IP) {
			instance->_WiFiConnected = true;

			auto event = (ip_event_got_ip_t*) eventData;
			ESP_LOGI("Transceiver (WiFi)", "Connected with ip " IPSTR, IP2STR(&event->ip_info.ip));
		}
	}

	void WiFiTransceiver::fillRemotePacket() {
		auto& rc = RC::getInstance();

		_remotePacket.throttle1 = rc.getThrottles()[0];
		_remotePacket.throttle2 = rc.getThrottles()[1];

		_remotePacket.ailerons = rc.getJoystickHorizontal().getMappedUint16Value();
		_remotePacket.elevator = rc.getJoystickVertical().getMappedUint16Value();

		_remotePacket.rudder = rc.getRing().getMappedUint16Value();
		_remotePacket.flaps = rc.getLeverRight().getMappedUint16Value();
		_remotePacket.spoilers = rc.getLeverLeft().getMappedUint16Value();

		_remotePacket.landingGear = true;
		_remotePacket.strobeLights = true;
	}
}