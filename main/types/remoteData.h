#pragma once

#include <cstdint>

namespace pizda {
	class RemoteDataRaw {
		public:
			float transceiverRSSIDBm = 0;
			uint8_t batteryVoltageMV = 0;
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