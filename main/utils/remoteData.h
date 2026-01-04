#pragma once

#include <cstdint>

#include "types.h"

namespace pizda {
	class RemoteDataLights {
		public:
			bool navigation = false;
			bool strobe = false;
			bool landing = false;
			bool cabin = false;
	};
	
	class RemoteDataAutopilot {
		public:
			AutopilotLateralMode lateralMode = AutopilotLateralMode::roll;
			AutopilotVerticalMode verticalMode = AutopilotVerticalMode::pitch;
			
			bool autothrottle = false;
			bool autopilot = false;
	};
	
	class RemoteData {
		public:
			uint8_t throttle_0_255 = 0;
			
			RemoteDataLights lights {};
			RemoteDataAutopilot autopilot {};
	};
}