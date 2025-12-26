#pragma once

#include <cstdint>
#include <YOBA/main.h>

namespace pizda {
	using namespace YOBA;

	class Statistics {
		public:
			uint32_t tickDeltaTime = 0;
			
			int64_t transmitterRXDurationUs = 0;
			int64_t transmitterTXDurationUs = 0;
	};
}