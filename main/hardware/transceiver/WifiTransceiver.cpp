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

		TCPTick();

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

			instance->TCPConnect();
		}
	}

	void WiFiTransceiver::TCPConnect() {
		ESP_LOGE("Transceiver (TCP)", "Creating socket");

		inet_pton(AF_INET, constants::transceiver::wifi::address, &_socketAddress.sin_addr);
		_socketAddress.sin_family = AF_INET;
		_socketAddress.sin_port = htons(constants::transceiver::wifi::port);

		_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

		if (_socket < 0) {
			ESP_LOGE("Transceiver (TCP)", "Unable to create socket: errno %d, %s", errno, strerror(errno));
			return;
		}

		ESP_LOGE("Transceiver (TCP)", "Connecting to socket");

		const auto err = connect(_socket, (struct sockaddr*) &_socketAddress, sizeof(_socketAddress));

		if (err != 0) {
			ESP_LOGE("Transceiver (TCP)", "Unable to connect: %d, %s", errno, strerror(errno));

			shutdown(_socket, 0);
			_socket = -1;

			return;
		}

		ESP_LOGI("Transceiver (TCP)", "Connected");
	}

	RemotePacket WiFiTransceiver::createRemotePacket() {
		auto& rc = RC::getInstance();

		return RemotePacket {
			.throttle1 = rc.getThrottles()[0],
			.throttle2 = rc.getThrottles()[1],

			.ailerons = rc.getJoystickHorizontal().getMappedUint16Value(),
			.elevator = rc.getJoystickVertical().getMappedUint16Value(),

			.rudder = rc.getRing().getMappedUint16Value(),
			.flaps = rc.getLeverRight().getMappedUint16Value(),
			.spoilers = rc.getLeverLeft().getMappedUint16Value(),

			.landingGear = true,
			.strobeLights = true
		};
	}

	void WiFiTransceiver::handlePacket(AircraftPacket* packet) {
//		packet.log();

		auto& rc = RC::getInstance();

		rc.getPitchInterpolator().setTargetValue(packet->pitch);
		rc.getRollInterpolator().setTargetValue(packet->roll);
		rc.getYawInterpolator().setTargetValue(packet->yaw);

		rc.getAltitudeInterpolator().setTargetValue(packet->altitude);
		rc.getSpeedInterpolator().setTargetValue(packet->speed);
	}

	void WiFiTransceiver::TCPSend() {
		const auto packet = createRemotePacket();

		const auto sendLength = send(_socket, &packet, sizeof(RemotePacket), 0);

		if (sendLength < 0) {
			ESP_LOGE("Transceiver (TCP)", "Error occurred during sending: %d, %s", errno, strerror(errno));
		}
	}

	void WiFiTransceiver::TCPReceive() {
		const size_t bufferLength = sizeof(AircraftPacket);
		uint8_t buffer[bufferLength];

		const auto receivedLength = recv(_socket, buffer, bufferLength, 0);

		if (receivedLength < 0) {
			ESP_LOGE("Transceiver (TCP)", "Receive failed: %d, %s", errno, strerror(errno));
			return;
		}
		else if (receivedLength != bufferLength) {
			ESP_LOGE("Transceiver (TCP)", "Received length %d != packet length %d", receivedLength, bufferLength);
			return;
		}

		handlePacket((AircraftPacket*) buffer);
	}

	void WiFiTransceiver::TCPTick() {
		if (!_WiFiConnected || _socket < 0)
			return;

		TCPSend();
		TCPReceive();
	}
}