#pragma once

#include <cstdint>
#include <esp_timer.h>
#include <esp_log.h>
#include <memory>
#include <cstring>
#include <ranges>

#include <YOBA/main.h>

#include "../hardware/NVS/NVSStream.h"
#include "../hardware/NVS/NVSSerializable.h"

namespace pizda {
	using namespace YOBA;

	enum class SettingsNavigationWaypointType : uint8_t {
		enroute,
		terminal,
		airport
	};

	class SettingsNavigationWaypointCoordinatesAware{
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
				SettingsNavigationWaypointType type,
				std::wstring_view nameArg,
				const GeographicCoordinates& coordinates
			) :
				SettingsNavigationWaypointCoordinatesAware(coordinates),
				type(type)
			{
				std::ranges::copy(nameArg, name);
			}

			SettingsNavigationWaypointType type;
			wchar_t name[16] {};
	};

	class SettingsNavigationRNAVWaypoint : public SettingsNavigationWaypointIndexAware {
		public:
			explicit SettingsNavigationRNAVWaypoint() = default;

			explicit SettingsNavigationRNAVWaypoint(uint16_t waypointIndex) :
				SettingsNavigationWaypointIndexAware(waypointIndex)
			{

			}
	};

	class SettingsNavigationAirportRunway : public SettingsNavigationWaypointIndexAware, public SettingsNavigationWaypointCoordinatesAware {
		public:
			explicit SettingsNavigationAirportRunway() = default;

			SettingsNavigationAirportRunway(
				uint16_t airportWaypointIndex,
				const GeographicCoordinates& coordinates,
				uint16_t headingDeg,
				uint16_t lengthM,
				uint16_t widthM
			) :
				SettingsNavigationWaypointIndexAware(airportWaypointIndex),
				SettingsNavigationWaypointCoordinatesAware(coordinates),
				headingDeg(headingDeg),
				lengthM(lengthM),
				widthM(widthM)
			{

			}

			uint16_t headingDeg = 0;
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
			std::vector<SettingsNavigationWaypoint> waypoints {};
			std::vector<SettingsNavigationRNAVWaypoint> RNAVWaypoints {};
			std::vector<SettingsNavigationAirportRunway> runways {};
			SettingsNavigationFlightPlan flightPlan {};

			size_t getOrCreateWaypointIndex(SettingsNavigationWaypointType type, std::wstring_view name, const GeographicCoordinates& coordinates) {
				const auto it = std::ranges::find_if(waypoints, [&name](const auto& wp) {
					return wp.name == name;
				});

				// Returning index of existing waypoint
				if (it != waypoints.end())
					return it - waypoints.begin();

				// Creating new waypoint
				waypoints.push_back(SettingsNavigationWaypoint(
					type,
					name,
					coordinates
				));

				return waypoints.size() - 1;
			}

		protected:
			const char* getNVSNamespace() override {
				return _namespace;
			}

			void onRead(const NVSStream& stream) override {
				// Waypoints
				waypoints.clear();
				RNAVWaypoints.clear();
				runways.clear();

				uint16_t size = stream.getUint16(_waypointsSize, 0);

				if (size > 0) {
					waypoints.reserve(size);

					stream.getBlob(
						_waypointsList,
						reinterpret_cast<uint8_t*>(waypoints.data()),
						sizeof(SettingsNavigationWaypoint) * size
					);

					// RNAV waypoints
					size = stream.getUint16(_RNAVWaypointsSize, 0);

					if (size > 0) {
						RNAVWaypoints.reserve(size);

						stream.getBlob(
							_RNAVWaypointsList,
							reinterpret_cast<uint8_t*>(RNAVWaypoints.data()),
							sizeof(SettingsNavigationRNAVWaypoint) * size
						);
					}

					// Runways
					size = stream.getUint16(_runwaysSize, 0);

					if (size > 0) {
						runways.reserve(size);

						stream.getBlob(
							_runwaysList,
							reinterpret_cast<uint8_t*>(runways.data()),
							sizeof(SettingsNavigationAirportRunway) * size
						);
					}
				}
				else {
					fillTemplateData();
				}
			}

			void onWrite(const NVSStream& stream) override {
				// Waypoints
				stream.setUint16(_waypointsSize, waypoints.size());

				stream.setBlob(
					_waypointsList,
					reinterpret_cast<uint8_t*>(waypoints.data()),
					sizeof(SettingsNavigationWaypoint) * waypoints.size()
				);

				// RNAV waypoints
				stream.setUint16(_RNAVWaypointsSize, RNAVWaypoints.size());

				stream.setBlob(
					_RNAVWaypointsList,
					reinterpret_cast<uint8_t*>(RNAVWaypoints.data()),
					sizeof(SettingsNavigationRNAVWaypoint) * RNAVWaypoints.size()
				);

				// Runways
				stream.setUint16(_runwaysSize, runways.size());

				stream.setBlob(
					_runwaysList,
					reinterpret_cast<uint8_t*>(runways.data()),
					sizeof(SettingsNavigationAirportRunway) * runways.size()
				);
			}

		private:
			constexpr static auto _namespace = "nvns";

			constexpr static auto _waypointsSize = "nvws";
			constexpr static auto _waypointsList = "nvwl";

			constexpr static auto _RNAVWaypointsSize = "nvns";
			constexpr static auto _RNAVWaypointsList = "nvnl";

			constexpr static auto _runwaysSize = "nvrs";
			constexpr static auto _runwaysList = "nvrl";

			void fillTemplateData() {
				RNAVWaypoints.reserve(6);
				runways.reserve(3);
				flightPlan.routeWaypointIndices.reserve(6);

				// Kronshtadt
				const auto departureAirportWaypointIndex = getOrCreateWaypointIndex(
					SettingsNavigationWaypointType::airport,
					L"ULLY",
					GeographicCoordinates(toRadians(60.014568277272f), toRadians(29.702727704862f), 0)
				);

				// 95
				runways.push_back(SettingsNavigationAirportRunway(
					departureAirportWaypointIndex,
					GeographicCoordinates(toRadians(60.014568277272f), toRadians(29.702727704862f), 0),
					95,
					500,
					30
				));

				const uint8_t departureRunwayIndex = runways.size() - 1;

				// RNAV waypoints

				RNAVWaypoints.push_back(SettingsNavigationRNAVWaypoint(getOrCreateWaypointIndex(
					SettingsNavigationWaypointType::enroute,
					L"OMEGA",
					GeographicCoordinates(toRadians(59.983333f), toRadians(30.133333f), 0)
				)));

				RNAVWaypoints.push_back(SettingsNavigationRNAVWaypoint(getOrCreateWaypointIndex(
					SettingsNavigationWaypointType::enroute,
					L"ABREL",
					GeographicCoordinates(toRadians(59.913056f), toRadians(31.335f), 0)
				)));

				RNAVWaypoints.push_back(SettingsNavigationRNAVWaypoint(getOrCreateWaypointIndex(
					SettingsNavigationWaypointType::enroute,
					L"SAPKI",
					GeographicCoordinates(toRadians(59.604722f), toRadians(31.180833f), 0)
				)));

				RNAVWaypoints.push_back(SettingsNavigationRNAVWaypoint(getOrCreateWaypointIndex(
					SettingsNavigationWaypointType::terminal,
					L"LI754",
					GeographicCoordinates(toRadians(59.516944f), toRadians(31.0225f), 0)
				)));

				RNAVWaypoints.push_back(SettingsNavigationRNAVWaypoint(getOrCreateWaypointIndex(
					SettingsNavigationWaypointType::terminal,
					L"OBARI",
					GeographicCoordinates(toRadians(59.599722f), toRadians(30.679167f), 0)
				)));

				RNAVWaypoints.push_back(SettingsNavigationRNAVWaypoint(getOrCreateWaypointIndex(
					SettingsNavigationWaypointType::terminal,
					L"BIPRI",
					GeographicCoordinates(toRadians(59.747778f), toRadians(30.565556f), 0)
				)));

				// Pulkovo
				const auto arrivalAirportWaypointIndex = getOrCreateWaypointIndex(
					SettingsNavigationWaypointType::airport,
					L"ULLI",
					GeographicCoordinates(toRadians(59.800278f), toRadians(30.2625f), 0)
				);

				// 10 L
				runways.push_back(SettingsNavigationAirportRunway(
					arrivalAirportWaypointIndex,
					GeographicCoordinates(toRadians(59.805114621892f), toRadians(30.276415586255f), 0),
					106,
					3780,
					60
				));

				// 10 R
				runways.push_back(SettingsNavigationAirportRunway(
					arrivalAirportWaypointIndex,
					GeographicCoordinates(toRadians(59.794929404415f), toRadians(30.251926678005f), 0),
					106,
					3780,
					60
				));

				const uint8_t arrivalRunwayIndex = runways.size() - 1;

				// Flight plan
				flightPlan.departureAirportWaypointIndex = departureAirportWaypointIndex;
				flightPlan.departureRunwayIndex = departureRunwayIndex;

				flightPlan.arrivalAirportWaypointIndex = arrivalAirportWaypointIndex;
				flightPlan.arrivalRunwayIndex = arrivalRunwayIndex;

				// Route
				flightPlan.routeWaypointIndices.push_back(1);
				flightPlan.routeWaypointIndices.push_back(2);
				flightPlan.routeWaypointIndices.push_back(3);
				flightPlan.routeWaypointIndices.push_back(4);
				flightPlan.routeWaypointIndices.push_back(5);
				flightPlan.routeWaypointIndices.push_back(6);

			}
	};
}
