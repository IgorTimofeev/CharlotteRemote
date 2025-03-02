#pragma once

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <netdb.h>
#include <arpa/inet.h>
#include "esp_netif.h"

#include "packet.h"
#include "../../constants.h"

namespace pizda {
	class WiFiTransceiver {
		public:
			void setup();

			void tick();

		private:
			uint32_t _tickInterval = 0;

			bool _WiFiConnected = false;

			int _socket = -1;
			struct sockaddr_in _socketAddress;

			static void WiFiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData);

			void WiFiSetup();

			void TCPConnect();

			void TCPTick();

			void TCPSend();

			RemotePacket newRemotePacket();

			void TCPReceive();
	};
}