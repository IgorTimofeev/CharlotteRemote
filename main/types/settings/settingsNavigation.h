#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>
#include <ranges>

#include <YOBA/main.h>

#include "types/navigationData.h"

#include <NVSSettings.h>

namespace pizda {
	using namespace YOBA;

	class SettingsNavigationWaypoint {
		public:
			SettingsNavigationWaypoint() = default;

			explicit SettingsNavigationWaypoint(
				NavigationWaypointType type,
				std::wstring_view name,
				const GeographicCoordinates& geographicCoordinates
			) :
				type(type),
				geographicCoordinates(geographicCoordinates)
			{
				std::ranges::copy(name, this->name);
			}

			NavigationWaypointType type;
			wchar_t name[16] {};
			GeographicCoordinates geographicCoordinates;
	};

	class SettingsNavigationRNAVWaypoint : public NavigationWaypointDataIndexAware {
		public:
			SettingsNavigationRNAVWaypoint() = default;

			explicit SettingsNavigationRNAVWaypoint(uint16_t waypointIndex) : NavigationWaypointDataIndexAware(waypointIndex) {\

			}
	};

	class SettingsNavigationAirport : public NavigationWaypointDataIndexAware {
		public:
			SettingsNavigationAirport() = default;

			explicit SettingsNavigationAirport(uint16_t waypointIndex) : NavigationWaypointDataIndexAware(waypointIndex) {

			}
	};

	class SettingsNavigationAirportRunway {
		public:
			SettingsNavigationAirportRunway() = default;

			SettingsNavigationAirportRunway(
				uint16_t airportIndex,
				const GeographicCoordinates& geographicCoordinates,
				uint16_t headingDeg,
				NavigationRunwayDataAlignment alignment,
				uint16_t lengthM,
				uint16_t widthM
			) :
				airportIndex(airportIndex),
				geographicCoordinates(geographicCoordinates),
				headingDeg(headingDeg),
				alignment(alignment),
				lengthM(lengthM),
				widthM(widthM)
			{

			}

			uint16_t airportIndex = 0;
			GeographicCoordinates geographicCoordinates;
			uint16_t headingDeg = 0;
			NavigationRunwayDataAlignment alignment;
			uint16_t lengthM = 0;
			uint16_t widthM = 0;
	};

	class SettingsNavigationFlightPlanLeg : public NavigationWaypointDataIndexAware {
		public:
			SettingsNavigationFlightPlanLeg() = default;

			explicit SettingsNavigationFlightPlanLeg(uint16_t waypointIndex) : NavigationWaypointDataIndexAware(waypointIndex) {

			}
	};

	class SettingsNavigation : public NVSSettings {
		public:

		protected:
			const char* getNamespace() override;

			void onRead(const NVSStream& stream) override;

			void onWrite(const NVSStream& stream) override;

		private:
			constexpr static auto _namespace = "nv";

			constexpr static auto _waypointsSize = "ws";
			constexpr static auto _waypointsList = "wl";

			constexpr static auto _RNAVWaypointsSize = "vs";
			constexpr static auto _RNAVWaypointsList = "vl";

			constexpr static auto _airportsSize = "as";
			constexpr static auto _airportsList = "al";

			constexpr static auto _runwaysSize = "rs";
			constexpr static auto _runwaysList = "rl";

			constexpr static auto _flightPlanOriginExists = "foe";
			constexpr static auto _flightPlanOriginAirportIndex = "foa";
			constexpr static auto _flightPlanOriginRunwayIndex = "for";

			constexpr static auto _flightPlanLegsSize = "fls";
			constexpr static auto _flightPlanLegsList = "fll";

			constexpr static auto _flightPlanDestinationExists = "fde";
			constexpr static auto _flightPlanDestinationAirportIndex = "fda";
			constexpr static auto _flightPlanDestinationRunwayIndex = "fdr";

			void fillTemplateData();
	};
}
