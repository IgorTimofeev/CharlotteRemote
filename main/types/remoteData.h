#pragma once

#include <cstdint>

#include "types/generic.h"

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
			float altitudeHoldFt = 0;
			
			AutopilotLateralMode lateralMode = AutopilotLateralMode::man;
			AutopilotVerticalMode verticalMode = AutopilotVerticalMode::man;
			
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