#pragma once

#include <optional>
#include <YOBA/main.h>

namespace pizda {
	using namespace YOBA;

	enum class NavigationWaypointType : uint8_t {
		enroute,
		terminal,
		airport
	};

	class NavigationWaypointData {
		public:
			NavigationWaypointData(
				NavigationWaypointType type,
				std::wstring_view name,
				const GeographicCoordinates& coordinates
			);

			NavigationWaypointType type;
			GeographicCoordinates geographicCoordinates {};
			wchar_t name[16] {};
			Vector3F cartesianCoordinates;
	};

	class NavigationRNAVWaypointData : public NavigationWaypointData {
		public:
			NavigationRNAVWaypointData(NavigationWaypointType type, const std::wstring_view& name, const GeographicCoordinates& coordinates) :
				NavigationWaypointData(type, name, coordinates)
			{

			}
	};

	enum class NavigationRunwayDataAlignment : uint8_t {
		none,
		left,
		center,
		right
	};

	class NavigationRunwayData {
		public:
			NavigationRunwayData(
				const GeographicCoordinates& coordinates,
				uint16_t headingDeg,
				NavigationRunwayDataAlignment alignment,
				uint16_t lengthM,
				uint16_t widthM
			);

			GeographicCoordinates coordinates {};
			uint16_t headingDeg = 0;
			NavigationRunwayDataAlignment alignment;
			uint16_t lengthM = 0;
			uint16_t widthM = 0;
			Vector3F vertices[5];

		private:
			Vector3F cornerToVertex(const Vector2F& corner) const;
	};

	class NavigationAirportData : public NavigationWaypointData {
		public:
			NavigationAirportData(const std::wstring_view& name, const GeographicCoordinates& coordinates, std::initializer_list<NavigationRunwayData> runways) :
				NavigationWaypointData(NavigationWaypointType::airport, name, coordinates),
				runways(runways)
			{

			}

			std::vector<NavigationRunwayData> runways {};
	};

	class NavigationDataFlightPlanAirport {
		public:
			NavigationDataFlightPlanAirport(const NavigationAirportData* airport, uint16_t runwayIndex) :
				airport(airport),
				runwayIndex(runwayIndex)
			{

			}

			const NavigationAirportData* airport;
			uint16_t runwayIndex = 0;
	};

	class NavigationDataFlightPlanRoute {
		public:
			NavigationDataFlightPlanRoute(
				const NavigationWaypointData* from,
				const NavigationWaypointData* to
			);

			const NavigationWaypointData* from;
			const NavigationWaypointData* to;
			Vector3F cartesianCoordinates[2];
	};

	class NavigationDataFlightPlan {
		public:
			std::optional<NavigationDataFlightPlanAirport> departure = std::nullopt;
			std::optional<NavigationDataFlightPlanAirport> arrival = std::nullopt;
			std::vector<NavigationDataFlightPlanRoute> routes {};
	};

	class NavigationData {
		public:
			std::vector<NavigationRNAVWaypointData> RNAVWaypoints {};
			std::vector<NavigationAirportData> airports {};
			NavigationDataFlightPlan flightPlan {};

			// size_t getOrCreateWaypointIndex(SettingsNavigationWaypointType type, std::wstring_view name, const GeographicCoordinates& coordinates) {
			// 	const auto it = std::ranges::find_if(waypoints, [&name](const auto& wp) {
			// 		return wp.name == name;
			// 	});
			//
			// 	// Returning index of existing waypoint
			// 	if (it != waypoints.end())
			// 		return it - waypoints.begin();
			//
			// 	// Creating new waypoint
			// 	waypoints.push_back(SettingsNavigationWaypoint(
			// 		type,
			// 		name,
			// 		coordinates
			// 	));
			//
			// 	return waypoints.size() - 1;
			// }

			void fillWithTemplateData();
	};
}