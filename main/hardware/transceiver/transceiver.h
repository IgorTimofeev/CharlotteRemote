#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_system.h>
#include <esp_log.h>

#include <SX1262Ex.h>

#include "config.h"
#include "hardware/transceiver/packet.h"

namespace pizda {
	using namespace YOBA;
	
	class Transceiver {
		public:
			void setup();
			void start();
			void stop();

		private:
			constexpr static const char* _logTag = "XCVR";
			
			SX1262Ex sx1262 {};
			
			bool _started = false;
			RemotePacket _remotePacket {};
			
			void fillRemotePacket();
			void onStart();
	};
}