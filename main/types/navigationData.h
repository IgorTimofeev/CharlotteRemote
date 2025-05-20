#pragma once

#include <optional>
#include <format>
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
			NavigationWaypointData() = default;

			NavigationWaypointData(
				NavigationWaypointType type,
				std::wstring_view name,
				const GeographicCoordinates& coordinates
			);

			NavigationWaypointType type;
			std::wstring name {};
			GeographicCoordinates geographicCoordinates {};
			Vector3F cartesianCoordinates;
	};

	class NavigationWaypointDataIndexAware {
		public:
			explicit NavigationWaypointDataIndexAware() = default;

			explicit NavigationWaypointDataIndexAware(uint16_t waypointIndex) : waypointIndex(waypointIndex) {

			}

			uint16_t waypointIndex;
	};

	class NavigationRNAVWaypointData : public NavigationWaypointDataIndexAware {
		public:
			NavigationRNAVWaypointData() = default;

			explicit NavigationRNAVWaypointData(uint16_t waypointIndex) :
				NavigationWaypointDataIndexAware(waypointIndex)
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
				const GeographicCoordinates& geographicCoordinates,
				uint16_t headingDeg,
				NavigationRunwayDataAlignment alignment,
				uint16_t lengthM,
				uint16_t widthM
			);

			GeographicCoordinates geographicCoordinates {};
			uint16_t headingDeg = 0;
			NavigationRunwayDataAlignment alignment;
			uint16_t lengthM = 0;
			uint16_t widthM = 0;
			Vector3F vertices[5];

			uint8_t getShortHeading() const {
				return (headingDeg % 10 >= 5 ? headingDeg + 10 : headingDeg) / 10;
			}

			std::wstring getFormattedName() const {
				switch (alignment) {
					case NavigationRunwayDataAlignment::left:
						return std::format(L"{:02}L", getShortHeading());

					case NavigationRunwayDataAlignment::center:
						return std::format(L"{:02}C", getShortHeading());

					case NavigationRunwayDataAlignment::right:
						return std::format(L"{:02}R", getShortHeading());

					default:
						return std::format(L"{:02}", getShortHeading());
				}
			}

		private:
			Vector3F cornerToVertex(const Vector2F& corner) const;
	};

	class NavigationAirportData : public NavigationWaypointDataIndexAware {
		public:
			NavigationAirportData(uint16_t waypointIndex, std::initializer_list<NavigationRunwayData> runways) :
				NavigationWaypointDataIndexAware(waypointIndex),
				runways(runways)
			{

			}

			std::vector<NavigationRunwayData> runways {};
	};

	class NavigationAirportIndexAndRunwayIndexData {
		public:
			NavigationAirportIndexAndRunwayIndexData(uint16_t airportIndex, uint16_t runwayIndex) :
				airportIndex(airportIndex),
				runwayIndex(runwayIndex)
			{

			}

			uint16_t airportIndex = 0;
			uint16_t runwayIndex = 0;
	};

	class NavigationDataFlightPlanAirport : public NavigationAirportIndexAndRunwayIndexData {
		public:
			NavigationDataFlightPlanAirport(uint16_t airportIndex, uint16_t runwayIndex) :
				NavigationAirportIndexAndRunwayIndexData(airportIndex, runwayIndex)
			{

			}
	};

	class NavigationDataFlightPlanLeg : public NavigationWaypointDataIndexAware {
		public:
			NavigationDataFlightPlanLeg() = default;

			explicit NavigationDataFlightPlanLeg(uint16_t waypointIndex);
	};

	class NavigationDataFlightPlan {
		public:
			std::optional<NavigationDataFlightPlanAirport> origin = std::nullopt;
			std::optional<NavigationDataFlightPlanAirport> destination = std::nullopt;
			std::vector<NavigationDataFlightPlanLeg> legs {};
	};

	class NavigationData {
		public:
			std::vector<NavigationWaypointData> waypoints {};
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

			void addAirport(std::wstring_view name, const GeographicCoordinates& coordinates, std::initializer_list<NavigationRunwayData> runways);
			void addRNAVWaypoint(NavigationWaypointType type, std::wstring_view name, const GeographicCoordinates& coordinates);
			void fillWithTemplateData();
			void removeWaypointAt(uint16_t waypointIndex);
			size_t getAirportIndex(uint16_t waypointIndex) const;

			size_t getRNAVWaypointIndex(uint16_t waypointIndex) const;

		private:
			template<std::derived_from<NavigationWaypointDataIndexAware> T>
			static int32_t getWaypointIndexAwareVectorIndex(const std::vector<T>& source, uint16_t waypointIndex);

			template<std::derived_from<NavigationWaypointDataIndexAware> T>
			static void shiftWaypointIndicesAt(std::vector<T>& source, uint16_t waypointIndex);
	};

	template<std::derived_from<NavigationWaypointDataIndexAware> T>
	int32_t NavigationData::getWaypointIndexAwareVectorIndex(const std::vector<T>& source, uint16_t waypointIndex) {
		for (uint16_t i = 0; i < source.size(); i++) {
			if (source[i].waypointIndex == waypointIndex) {
				return i;
			}
		}

		return -1;
	}

	template<std::derived_from<NavigationWaypointDataIndexAware> T>
	void NavigationData::shiftWaypointIndicesAt(std::vector<T>& source, uint16_t waypointIndex) {
		for (int32_t i = 0; i < source.size(); i++) {
			auto& item = source[i];

			if (item.waypointIndex == waypointIndex) {
				source.erase(source.begin() + i);
				i--;
			}
			else if (item.waypointIndex > waypointIndex) {
				// ReSharper disable once CppDiscardedPostfixOperatorResult
				item.waypointIndex--;
			}
		}
	}
}
