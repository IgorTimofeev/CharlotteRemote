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
		RNAV,
		airport
	};

	class SettingsNavigationWaypoint {
		public:
			SettingsNavigationWaypoint() = default;

			explicit SettingsNavigationWaypoint(SettingsNavigationWaypointType type, std::wstring_view nameArg) : type(type) {
				std::ranges::copy(nameArg, name);
			}

			SettingsNavigationWaypointType type;
			wchar_t name[16] {};
	};

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

			explicit SettingsNavigationWaypointIndexAware(uint16_t waypointIndex) : waypointIndex(waypointIndex) {

			}

			uint16_t waypointIndex = 0;
	};

	class SettingsNavigationRNAVWaypoint : public SettingsNavigationWaypointIndexAware, public SettingsNavigationWaypointCoordinatesAware {
		public:
			explicit SettingsNavigationRNAVWaypoint() = default;

			explicit SettingsNavigationRNAVWaypoint(uint16_t waypointIndex, const GeographicCoordinates& coordinates) :
				SettingsNavigationWaypointIndexAware(waypointIndex),
				SettingsNavigationWaypointCoordinatesAware(coordinates)
			{

			}
	};

	class SettingsNavigationRunway : public SettingsNavigationWaypointIndexAware, public SettingsNavigationWaypointCoordinatesAware {
		public:
			explicit SettingsNavigationRunway() = default;

			SettingsNavigationRunway(uint16_t waypointIndex, const GeographicCoordinates& coordinates, uint16_t headingDeg, uint16_t lengthM, uint16_t widthM) :
				SettingsNavigationWaypointIndexAware(waypointIndex),
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

	class SettingsNavigation : public NVSSerializable {
		public:
			uint8_t throttle = 0;
			std::vector<SettingsNavigationWaypoint> waypoints {};
			std::vector<SettingsNavigationRNAVWaypoint> RNAVWaypoints {};
			std::vector<SettingsNavigationRunway> runways {};

			size_t getOrCreateWaypointIndex(SettingsNavigationWaypointType type, std::wstring_view name) {
				const auto it = std::ranges::find_if(waypoints, [&name](const auto& wp) {
					return wp.name == name;
				});

				// Returning index of existing waypoint
				if (it != waypoints.end())
					return it - waypoints.begin();

				// Creating new waypoint
				waypoints.push_back(SettingsNavigationWaypoint(
					type,
					name
				));

				return waypoints.size() - 1;
			}

			void addRNAVWaypoint(std::wstring_view name, const GeographicCoordinates& coordinates) {
				RNAVWaypoints.push_back(SettingsNavigationRNAVWaypoint(
					getOrCreateWaypointIndex(SettingsNavigationWaypointType::RNAV, name),
					coordinates
				));
			}

			void addRunway(std::wstring_view name, const GeographicCoordinates& coordinates, uint16_t headingDeg, uint16_t lengthM, uint16_t widthM) {
				runways.push_back(SettingsNavigationRunway(
					getOrCreateWaypointIndex(SettingsNavigationWaypointType::airport, name),
					coordinates,
					headingDeg,
					lengthM,
					widthM
				));
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
							sizeof(SettingsNavigationRunway) * size
						);
					}
				}
				// Filling with template data
				else {
					// RNAV waypoints
					addRNAVWaypoint(
						L"DIKOM",
						GeographicCoordinates(toRadians(59.79f), toRadians(29.76f), 0)
					);

					addRNAVWaypoint(
						L"LI29",
						GeographicCoordinates(toRadians(60.06f), toRadians(29.63f), 0)
					);

					// Runways


					// Kronshtadt
					addRunway(
						L"ULLY",
						GeographicCoordinates(toRadians(60.014568277272f), toRadians(29.702727704862f), 0),
						95,
						500,
						30
					);

					// Pulkovo 10 L
					addRunway(
						L"ULLI",
						GeographicCoordinates(toRadians(59.805114621892f), toRadians(30.276415586255f), 0),
						106,
						3780,
						60
					);

					// Pulkovo 10 R
					addRunway(
						L"ULLI",
						GeographicCoordinates(toRadians(59.794929404415f), toRadians(30.251926678005f), 0),
						106,
						3780,
						60
					);
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
					sizeof(SettingsNavigationRunway) * runways.size()
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
	};
}
