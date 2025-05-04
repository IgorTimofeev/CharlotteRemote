#pragma once

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "packet.h"
#include "../TCP.h"
#include "../WiFi.h"
#include "../../constants.h"

namespace pizda {
	class TCPTransceiver {
		public:
			void setup();
			void start();
			void stop();
			bool isStarted();

		private:
			TaskHandle_t* _task = nullptr;

			TCP _TCP;

			RemotePacket _remotePacket;
			AircraftPacket _aircraftPacket;

			static void startTaskFunction(void* arg);

			void fillRemotePacket();
			void setTCPSendingBuffer();
			void setTCPReceivingBuffer();
	};
}