#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>
#include <ranges>

#include <YOBA/main.h>

#include "../types/navigationData.h"
#include "../hardware/NVS/NVSStream.h"
#include "../hardware/NVS/NVSSerializable.h"

namespace pizda {
	using namespace YOBA;

	class SettingsNavigationWaypointCoordinatesAware {
		public:
			explicit SettingsNavigationWaypointCoordinatesAware() = default;

			explicit SettingsNavigationWaypointCoordinatesAware(const GeographicCoordinates& coordinates) : coordinates(coordinates) {

			}

			GeographicCoordinates coordinates {};
	};

	class SettingsNavigationWaypointIndexAware {
		public:
			explicit SettingsNavigationWaypointIndexAware() = default;

			explicit SettingsNavigationWaypointIndexAware(uint16_t waypointIndex) :
				waypointIndex(waypointIndex)
			{

			}

			uint16_t waypointIndex = 0;
	};

	class SettingsNavigationWaypoint : public SettingsNavigationWaypointCoordinatesAware {
		public:
			SettingsNavigationWaypoint() = default;

			explicit SettingsNavigationWaypoint(
				NavigationWaypointType type,
				std::wstring_view name,
				const GeographicCoordinates& coordinates
			) :
				SettingsNavigationWaypointCoordinatesAware(coordinates),
				type(type)
			{
				std::ranges::copy(name, this->name);
			}

			NavigationWaypointType type;
			wchar_t name[16] {};
	};

	class SettingsNavigationAirportRunway : public SettingsNavigationWaypointIndexAware, public SettingsNavigationWaypointCoordinatesAware {
		public:
			SettingsNavigationAirportRunway() = default;

			SettingsNavigationAirportRunway(
				uint16_t airportWaypointIndex,
				const GeographicCoordinates& coordinates,
				uint16_t headingDeg,
				NavigationRunwayDataAlignment alignment,
				uint16_t lengthM,
				uint16_t widthM
			) :
				SettingsNavigationWaypointIndexAware(airportWaypointIndex),
				SettingsNavigationWaypointCoordinatesAware(coordinates),
				headingDeg(headingDeg),
				alignment(alignment),
				lengthM(lengthM),
				widthM(widthM)
			{

			}

			uint16_t headingDeg = 0;
			NavigationRunwayDataAlignment alignment;
			uint16_t lengthM = 0;
			uint16_t widthM = 0;
	};

	class SettingsNavigationFlightPlan {
		public:
			uint16_t departureAirportWaypointIndex = 0;
			uint8_t departureRunwayIndex = 0;

			uint16_t arrivalAirportWaypointIndex = 0;
			uint8_t arrivalRunwayIndex = 0;

			std::vector<uint16_t> routeWaypointIndices {};
	};

	class SettingsNavigation : public NVSSerializable {
		public:


		protected:
			const char* getNVSNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override;

			void onWrite(const NVSStream& stream) override;

		private:
			constexpr static auto _namespace = "nvns";

			constexpr static auto _waypointsSize = "nvws";
			constexpr static auto _waypointsList = "nvwl";

			constexpr static auto _runwaysSize = "nvrs";
			constexpr static auto _runwaysList = "nvrl";

			void fillTemplateData();
	};
}
