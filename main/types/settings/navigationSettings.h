#pragma once

#include <memory>
#include <cstring>
#include <ranges>

#include <YOBA/main.h>

#include <NVSSettings.h>

#include "types/navigationData.h"

namespace pizda {
	using namespace YOBA;

	class NavigationSettingsWaypoint {
		public:
			NavigationSettingsWaypoint() = default;

			explicit NavigationSettingsWaypoint(
				const NavigationWaypointType type,
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

	class NavigationSettingsEnrouteWaypoint : public NavigationWaypointDataIndexAware {
		public:
			NavigationSettingsEnrouteWaypoint() = default;

			explicit NavigationSettingsEnrouteWaypoint(const uint16_t waypointIndex) : NavigationWaypointDataIndexAware(waypointIndex) {\

			}
	};

	class NavigationSettingsRunway : public NavigationWaypointDataIndexAware {
		public:
			NavigationSettingsRunway() = default;

			explicit NavigationSettingsRunway(
				const uint16_t waypointIndex,
				const uint16_t headingDeg,
				const uint16_t lengthM,
				const uint16_t widthM
			) :
				NavigationWaypointDataIndexAware(waypointIndex),
				headingDeg(headingDeg),
				lengthM(lengthM),
				widthM(widthM)
			{

			}

			uint16_t headingDeg = 0;
			uint16_t lengthM = 0;
			uint16_t widthM = 0;
	};

	class NavigationSettingsFlightPlanLeg : public NavigationWaypointDataIndexAware {
		public:
			NavigationSettingsFlightPlanLeg() = default;

			explicit NavigationSettingsFlightPlanLeg(const uint16_t waypointIndex) : NavigationWaypointDataIndexAware(waypointIndex) {

			}
	};

	class NavigationSettings : public NVSSettings {
		protected:
			const char* getNamespace() override;

			void onRead(const NVSStream& stream) override;
			void onWrite(const NVSStream& stream) override;

		private:
			constexpr static auto _namespace = "nv2";

			constexpr static auto _waypointsList = "wpll";
			constexpr static auto _enrouteWaypointsList = "wpel";
			constexpr static auto _runwaysList = "wprl";
			constexpr static auto _flightPlanLegsList = "fpll";
			constexpr static auto _flightPlanActiveLegIndex = "fpal";
	};
}
