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
				// Kronshtadt
				Runway(
					GeographicCoordinates(toRadians(60.014568277272f), toRadians(29.702727704862f), 0),
					95,
					500,
					30,
					L"ULLY"
				),
				// 10L
				Runway(
					GeographicCoordinates(toRadians(59.805114621892f), toRadians(30.276415586255f), 0),
					106,
					3780,
					60,
					L"ULLI"
				),
				// 10R
				Runway(
					GeographicCoordinates(toRadians(59.794929404415f), toRadians(30.251926678005f), 0),
					106,
					3780,
					60,
					L"ULLI"
				)
			};
	};
}