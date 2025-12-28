#pragma once

#include <cstdint>

namespace pizda {
	class RemoteDataRaw {
		public:
			uint8_t throttle_0_255 = 0;
			
			bool navigationLights = false;
			bool strobeLights = false;
			bool landingLights = false;
			bool cabinLights = false;
	};
	
	class RemoteDataComputed {
		public:
			float transceiverRSSIDBm = 0;
	};

	class RemoteData {
		public:
			RemoteDataRaw raw {};
			RemoteDataComputed computed {};
	};
}