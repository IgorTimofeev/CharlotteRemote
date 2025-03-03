#pragma once

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "packet.h"
#include "TCPClient.h"
#include "../../constants.h"

namespace pizda {
	class WiFiTransceiver {
		public:
			void setup();

			void tick();

		private:
			uint32_t _tickInterval = 0;
			bool _WiFiConnected = false;

			RemotePacket _remotePacket {};
			AircraftPacket _aircraftPacket {};

			TCPClient _tcp = TCPClient(
				constants::transceiver::wifi::address,
				constants::transceiver::wifi::port
			);

			static void WiFiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData);

			void WiFiSetup();

			void fillRemotePacket();

			void handlePacket(AircraftPacket* packet);
	};
}