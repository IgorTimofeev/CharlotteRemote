#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <SX1262Ex.h>
#include <bitStream.h>

#include "config.h"
#include "hardware/transceiver/packet.h"

namespace pizda {
	using namespace YOBA;
	
	class Transceiver {
		public:
			void setup();
			void start();
			void stop();
			
			float getRSSI();

		private:
			constexpr static const char* _logTag = "XCVR";
			
			
			SX1262Ex sx1262 {};
			
			bool _setupValid = false;
			bool _started = false;
			RemotePacket _remotePacket {};
			uint64_t _aircraftPacketTime = 0;
			float _RSSI = 0;
			
			void fillRemotePacket();
			void onStart();
			void parseAircraftPacket(BitStream& bitStream);
	};
}