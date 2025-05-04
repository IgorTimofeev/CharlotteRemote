#pragma once

#include <cstdint>
#include <vector>
#include "YOBA/main.h"
#include "runway.h"

namespace pizda {
	using namespace YOBA;

	class SpatialData {
		public:
			std::vector<Runway> runways = {
				Runway(
					GeographicCoordinates(toRadians(60.014581566191914f), toRadians(29.70258579817704f), 0),
					95,
					500,
					30,
					L"ULLY"
				),
				Runway(
					GeographicCoordinates(toRadians(59.79507652101131f), toRadians(30.250945449842572), 0),
					100,
					3780,
					60,
					L"ULLI"
				)
			};
	};
}