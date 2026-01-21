#pragma once

#include <optional>
#include <format>

#include <YOBA/main.h>

namespace pizda {
	using namespace YOBA;

	enum class NavigationWaypointType : uint8_t {
		enroute,
		runway
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

			explicit NavigationWaypointDataIndexAware(const uint16_t waypointIndex) : waypointIndex(waypointIndex) {

			}

			uint16_t waypointIndex;
	};

	class NavigationEnrouteWaypointData : public NavigationWaypointDataIndexAware {
		public:
			NavigationEnrouteWaypointData() = default;

			explicit NavigationEnrouteWaypointData(const uint16_t waypointIndex) :
				NavigationWaypointDataIndexAware(waypointIndex)
			{

			}
	};

	class NavigationRunwayData : public NavigationWaypointDataIndexAware {
		public:
			NavigationRunwayData(
				uint16_t waypointIndex,
				uint16_t headingDeg,
				uint16_t lengthM,
				uint16_t widthM
			);

			uint16_t headingDeg = 0;
			uint16_t lengthM = 0;
			uint16_t widthM = 0;
			Vector3F spatialVertices[4];

			uint8_t getShortHeading() const {
				return (headingDeg % 10 >= 5 ? headingDeg + 10 : headingDeg) / 10;
			}

			std::wstring getFormattedName() const {
				return std::format(L"{:02}", getShortHeading());
			}

		private:
			Vector3F cornerToVertex(const GeographicCoordinates& geographicCoordinates, const Vector2F& cornerRad) const;

	};

	class NavigationDataFlightPlanLeg : public NavigationWaypointDataIndexAware {
		public:
			NavigationDataFlightPlanLeg() = default;

			explicit NavigationDataFlightPlanLeg(uint16_t waypointIndex);
	};

	class NavigationDataFlightPlan {
		public:
			std::vector<NavigationDataFlightPlanLeg> legs {};
	};

	class NavigationData {
		public:
			std::vector<NavigationWaypointData> waypoints {};
			std::vector<NavigationEnrouteWaypointData> enrouteWaypoints {};
			std::vector<NavigationRunwayData> runways {};
			NavigationDataFlightPlan flightPlan {};

			void addEnrouteWaypoint(NavigationWaypointType type, std::wstring_view name, const GeographicCoordinates& coordinates);

			void addRunway(
				std::wstring_view name,
				const GeographicCoordinates& coordinates,
				uint16_t headingDeg,
				uint16_t lengthM,
				uint16_t widthM
			);

			void clear();
			void addTemplateData();
			void removeWaypointAt(uint16_t waypointIndex);
			size_t getRunwayIndex(uint16_t waypointIndex) const;
			size_t getEnrouteWaypointIndex(uint16_t waypointIndex) const;

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
